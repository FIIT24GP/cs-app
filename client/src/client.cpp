#include "client.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>

namespace cs_app {
namespace client {

Client::Client() : connected_(false) {
    server1_client_ = std::make_unique<network::TCPClient>("localhost", 8081);
    server2_client_ = std::make_unique<network::TCPClient>("localhost", 8082);
}

Client::~Client() {
    disconnect();
}

bool Client::connect() {
    if (connected_) {
        return true;
    }
    
    // Подключение к серверам
    if (!server1_client_->connect()) {
        std::cerr << "Failed to connect to server1" << std::endl;
        return false;
    }
    
    if (!server2_client_->connect()) {
        std::cerr << "Failed to connect to server2" << std::endl;
        server1_client_->disconnect();
        return false;
    }
    
    connected_ = true;
    return true;
}

void Client::disconnect() {
    if (server1_client_) {
        server1_client_->disconnect();
    }
    
    if (server2_client_) {
        server2_client_->disconnect();
    }
    
    connected_ = false;
}

bool Client::isConnected() const {
    return connected_;
}

std::vector<protocol::DiskInfo> Client::getDiskInfo() {
    if (!connected_) {
        throw std::runtime_error("Not connected to servers");
    }
    
    std::string response = sendRequest(protocol::Command::GET_DISK_INFO, "");
    return serialization::deserialize_disk_info_vector(response);
}

protocol::FilesystemInfo Client::getFilesystemInfo() {
    if (!connected_) {
        throw std::runtime_error("Not connected to servers");
    }
    
    std::string response = sendRequest(protocol::Command::GET_FILESYSTEM_INFO, "");
    return serialization::deserialize_filesystem_info(response);
}

protocol::KeyboardInfo Client::getKeyboardInfo() {
    if (!connected_) {
        throw std::runtime_error("Not connected to servers");
    }
    
    std::string response = sendRequest(protocol::Command::GET_KEYBOARD_INFO, "");
    return serialization::deserialize_keyboard_info(response);
}

protocol::ProcessInfo Client::getProcessInfo() {
    if (!connected_) {
        throw std::runtime_error("Not connected to servers");
    }
    
    std::string response = sendRequest(protocol::Command::GET_PROCESS_INFO, "");
    return serialization::deserialize_process_info(response);
}

std::vector<protocol::ThreadInfo> Client::getThreadInfo() {
    if (!connected_) {
        throw std::runtime_error("Not connected to servers");
    }
    
    std::string response = sendRequest(protocol::Command::GET_THREAD_INFO, "");
    return serialization::deserialize_thread_info_vector(response);
}

bool Client::sendRequest(protocol::Command command, const std::string& data) {
    // Создание заголовка сообщения
    protocol::MessageHeader header;
    header.magic = 0xDEADBEEF;
    header.length = static_cast<uint32_t>(data.length());
    header.type = protocol::MessageType::REQUEST;
    header.command = command;
    header.request_id = static_cast<uint32_t>(utils::get_current_timestamp());
    header.timestamp = utils::get_current_timestamp();
    
    // Сериализация сообщения
    std::string message = protocol::serialize_message(header, data);
    
    // Отправка запроса
    if (command == protocol::Command::GET_DISK_INFO || 
        command == protocol::Command::GET_FILESYSTEM_INFO || 
        command == protocol::Command::GET_KEYBOARD_INFO) {
        return server1_client_->send_message(message);
    } else {
        return server2_client_->send_message(message);
    }
}

std::string Client::receiveResponse() {
    std::string response;
    
    // Попытка получения ответа от server1
    if (server1_client_->receive_message(response)) {
        return response;
    }
    
    // Попытка получения ответа от server2
    if (server2_client_->receive_message(response)) {
        return response;
    }
    
    throw std::runtime_error("Failed to receive response from servers");
}

} // namespace client
} // namespace cs_app
