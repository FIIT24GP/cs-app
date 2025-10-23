#include "network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <thread>

namespace cs_app {
namespace network {

// TCPServer implementation
TCPServer::TCPServer(uint16_t port) : port_(port), server_socket_(-1), running_(false) {
}

TCPServer::~TCPServer() {
    stop();
}

bool TCPServer::start() {
    if (running_) {
        return true;
    }
    
    // Создание сокета
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    // Настройка адреса
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);
    
    // Привязка сокета
    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(server_socket_);
        return false;
    }
    
    // Прослушивание
    if (listen(server_socket_, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(server_socket_);
        return false;
    }
    
    running_ = true;
    
    // Запуск потока для принятия соединений
    std::thread accept_thread(&TCPServer::accept_connections, this);
    accept_thread.detach();
    
    return true;
}

void TCPServer::stop() {
    if (!running_) {
        return;
    }
    
    running_ = false;
    
    if (server_socket_ != -1) {
        close(server_socket_);
        server_socket_ = -1;
    }
}

void TCPServer::set_message_handler(MessageHandler handler) {
    std::lock_guard<std::mutex> lock(handler_mutex_);
    message_handler_ = handler;
}

void TCPServer::accept_connections() {
    while (running_) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            if (running_) {
                std::cerr << "Failed to accept connection" << std::endl;
            }
            continue;
        }
        
        // Запуск потока для обработки клиента
        std::thread client_thread(&TCPServer::handle_client, this, client_socket);
        client_thread.detach();
    }
}

void TCPServer::handle_client(int client_socket) {
    char buffer[4096];
    
    while (running_) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break;
        }
        
        buffer[bytes_received] = '\0';
        std::string request(buffer, bytes_received);
        std::string response;
        
        // Обработка сообщения
        {
            std::lock_guard<std::mutex> lock(handler_mutex_);
            if (message_handler_) {
                message_handler_(request, response);
            }
        }
        
        // Отправка ответа
        if (!response.empty()) {
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
    
    close(client_socket);
}

// TCPClient implementation
TCPClient::TCPClient(const std::string& host, uint16_t port) 
    : host_(host), port_(port), socket_(-1), connected_(false) {
}

TCPClient::~TCPClient() {
    disconnect();
}

bool TCPClient::connect() {
    if (connected_) {
        return true;
    }
    
    // Создание сокета
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    // Настройка адреса
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    
    if (inet_pton(AF_INET, host_.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(socket_);
        return false;
    }
    
    // Подключение
    if (::connect(socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        close(socket_);
        return false;
    }
    
    connected_ = true;
    return true;
}

void TCPClient::disconnect() {
    if (connected_ && socket_ != -1) {
        close(socket_);
        socket_ = -1;
        connected_ = false;
    }
}

bool TCPClient::send_message(const std::string& message) {
    if (!connected_) {
        return false;
    }
    
    ssize_t bytes_sent = send(socket_, message.c_str(), message.length(), 0);
    return bytes_sent == static_cast<ssize_t>(message.length());
}

bool TCPClient::receive_message(std::string& message) {
    if (!connected_) {
        return false;
    }
    
    char buffer[4096];
    ssize_t bytes_received = recv(socket_, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received <= 0) {
        return false;
    }
    
    buffer[bytes_received] = '\0';
    message = std::string(buffer, bytes_received);
    return true;
}

// FIFOHandler implementation
FIFOHandler::FIFOHandler(const std::string& fifo_path) 
    : fifo_path_(fifo_path), fifo_fd_(-1), is_reader_(false), is_writer_(false) {
}

FIFOHandler::~FIFOHandler() {
    close();
}

bool FIFOHandler::create_fifo() {
    if (mkfifo(fifo_path_.c_str(), 0666) == -1) {
        if (errno != EEXIST) {
            std::cerr << "Failed to create FIFO: " << strerror(errno) << std::endl;
            return false;
        }
    }
    return true;
}

bool FIFOHandler::open_for_reading() {
    if (is_reader_) {
        return true;
    }
    
    fifo_fd_ = open(fifo_path_.c_str(), O_RDONLY | O_NONBLOCK);
    if (fifo_fd_ == -1) {
        std::cerr << "Failed to open FIFO for reading: " << strerror(errno) << std::endl;
        return false;
    }
    
    is_reader_ = true;
    return true;
}

bool FIFOHandler::open_for_writing() {
    if (is_writer_) {
        return true;
    }
    
    fifo_fd_ = open(fifo_path_.c_str(), O_WRONLY | O_NONBLOCK);
    if (fifo_fd_ == -1) {
        std::cerr << "Failed to open FIFO for writing: " << strerror(errno) << std::endl;
        return false;
    }
    
    is_writer_ = true;
    return true;
}

bool FIFOHandler::write_message(const std::string& message) {
    if (!is_writer_) {
        std::cerr << "FIFO not opened for writing" << std::endl;
        return false;
    }
    
    ssize_t bytes_written = write(fifo_fd_, message.c_str(), message.length());
    if (bytes_written == -1) {
        std::cerr << "Failed to write to FIFO: " << strerror(errno) << std::endl;
        return false;
    }
    
    return bytes_written == static_cast<ssize_t>(message.length());
}

bool FIFOHandler::read_message(std::string& message) {
    if (!is_reader_) {
        std::cerr << "FIFO not opened for reading" << std::endl;
        return false;
    }
    
    char buffer[1024];
    ssize_t bytes_read = read(fifo_fd_, buffer, sizeof(buffer) - 1);
    
    if (bytes_read == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }
        std::cerr << "Failed to read from FIFO: " << strerror(errno) << std::endl;
        return false;
    }
    
    if (bytes_read == 0) {
        return false;
    }
    
    buffer[bytes_read] = '\0';
    message = std::string(buffer);
    return true;
}

void FIFOHandler::close() {
    if (fifo_fd_ != -1) {
        ::close(fifo_fd_);
        fifo_fd_ = -1;
    }
    
    is_reader_ = false;
    is_writer_ = false;
}

} // namespace network
} // namespace cs_app
