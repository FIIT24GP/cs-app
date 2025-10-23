#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace cs_app {
namespace network {

// Класс для TCP сервера
class TCPServer {
public:
    using MessageHandler = std::function<void(const std::string&, std::string&)>;
    
    TCPServer(uint16_t port);
    ~TCPServer();
    
    bool start();
    void stop();
    void set_message_handler(MessageHandler handler);
    
private:
    void accept_connections();
    void handle_client(int client_socket);
    
    uint16_t port_;
    int server_socket_;
    std::atomic<bool> running_;
    std::vector<std::thread> client_threads_;
    MessageHandler message_handler_;
    std::mutex handler_mutex_;
};

// Класс для TCP клиента
class TCPClient {
public:
    TCPClient(const std::string& host, uint16_t port);
    ~TCPClient();
    
    bool connect();
    void disconnect();
    bool send_message(const std::string& message);
    bool receive_message(std::string& message);
    
private:
    std::string host_;
    uint16_t port_;
    int socket_;
    bool connected_;
};

// Класс для работы с именованными каналами (FIFO)
class FIFOHandler {
public:
    FIFOHandler(const std::string& fifo_path);
    ~FIFOHandler();
    
    bool create_fifo();
    bool open_for_reading();
    bool open_for_writing();
    bool write_message(const std::string& message);
    bool read_message(std::string& message);
    void close();
    
private:
    std::string fifo_path_;
    int fifo_fd_;
    bool is_reader_;
    bool is_writer_;
};

} // namespace network
} // namespace cs_app
