#include "server1.h"
#include "utils.h"
#include <iostream>
#include <sstream>

namespace cs_app {
namespace server1 {

Server1::Server1() : running_(false) {
    tcp_server_ = std::make_unique<network::TCPServer>(8081);
    fifo_handler_ = std::make_unique<network::FIFOHandler>("/tmp/server1.log.fifo");
}

Server1::~Server1() {
    stop();
}

bool Server1::start() {
    if (running_) {
        return true;
    }
    
    // Создание именованного канала для логов
    if (!fifo_handler_->create_fifo()) {
        std::cerr << "Failed to create FIFO for logging" << std::endl;
        return false;
    }
    
    // Установка обработчика сообщений
    tcp_server_->set_message_handler([this](const std::string& request, std::string& response) {
        handle_message(request, response);
    });
    
    // Запуск TCP сервера
    if (!tcp_server_->start()) {
        std::cerr << "Failed to start TCP server" << std::endl;
        return false;
    }
    
    running_ = true;
    return true;
}

void Server1::stop() {
    if (!running_) {
        return;
    }
    
    tcp_server_->stop();
    fifo_handler_->close();
    running_ = false;
}

void Server1::wait() {
    if (tcp_server_) {
        // В реальной реализации здесь должно быть ожидание завершения сервера
        // Для простоты используем бесконечный цикл
        while (running_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void Server1::handle_message(const std::string& request, std::string& response) {
    try {
        protocol::MessageHeader header;
        std::string payload;
        
        if (!protocol::deserialize_message(request, header, payload)) {
            std::cerr << "Failed to deserialize message" << std::endl;
            return;
        }
        
        // Обработка различных команд
        switch (header.command) {
            case protocol::Command::GET_DISK_INFO:
                response = process_disk_info_request();
                break;
            case protocol::Command::GET_FILESYSTEM_INFO:
                response = process_filesystem_info_request();
                break;
            case protocol::Command::GET_KEYBOARD_INFO:
                response = process_keyboard_info_request();
                break;
            default:
                std::cerr << "Unknown command: " << static_cast<int>(header.command) << std::endl;
                return;
        }
        
        // Логирование запроса
        std::string log_message = "Server1: Processed command " + 
                                 std::to_string(static_cast<int>(header.command)) + 
                                 " at " + utils::get_current_time_string();
        fifo_handler_->write_message(log_message);
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling message: " << e.what() << std::endl;
    }
}

std::string Server1::process_disk_info_request() {
    try {
        auto disk_devices = utils::get_disk_devices();
        std::vector<protocol::DiskInfo> disk_infos;
        
        for (const auto& device : disk_devices) {
            protocol::DiskInfo info;
            info.device_name = device;
            info.mount_point = "/mnt/" + device; // Упрощенная реализация
            info.filesystem_type = utils::get_filesystem_type(device);
            info.total_size = utils::get_disk_size(device);
            info.free_size = utils::get_disk_free_space(device);
            
            disk_infos.push_back(info);
        }
        
        return serialization::serialize_disk_info_vector(disk_infos);
        
    } catch (const std::exception& e) {
        std::cerr << "Error processing disk info: " << e.what() << std::endl;
        return "";
    }
}

std::string Server1::process_filesystem_info_request() {
    try {
        protocol::FilesystemInfo info;
        info.type = "ext4"; // Упрощенная реализация
        info.mount_point = "/";
        info.block_size = 4096;
        info.total_blocks = 1000000;
        info.free_blocks = 500000;
        
        return serialization::serialize_filesystem_info(info);
        
    } catch (const std::exception& e) {
        std::cerr << "Error processing filesystem info: " << e.what() << std::endl;
        return "";
    }
}

std::string Server1::process_keyboard_info_request() {
    try {
        protocol::KeyboardInfo info;
        info.layout = utils::get_keyboard_layout();
        info.variant = utils::get_keyboard_variant();
        info.model = utils::get_keyboard_model();
        
        return serialization::serialize_keyboard_info(info);
        
    } catch (const std::exception& e) {
        std::cerr << "Error processing keyboard info: " << e.what() << std::endl;
        return "";
    }
}

} // namespace server1
} // namespace cs_app

