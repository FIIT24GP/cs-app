#include "server2.h"
#include "utils.h"
#include <iostream>
#include <sstream>

namespace cs_app {
namespace server2 {

Server2::Server2() : running_(false) {
    tcp_server_ = std::make_unique<network::TCPServer>(8082);
    fifo_handler_ = std::make_unique<network::FIFOHandler>("/tmp/server2.log.fifo");
}

Server2::~Server2() {
    stop();
}

bool Server2::start() {
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

void Server2::stop() {
    if (!running_) {
        return;
    }
    
    tcp_server_->stop();
    fifo_handler_->close();
    running_ = false;
}

void Server2::wait() {
    if (tcp_server_) {
        // В реальной реализации здесь должно быть ожидание завершения сервера
        // Для простоты используем бесконечный цикл
        while (running_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void Server2::handle_message(const std::string& request, std::string& response) {
    try {
        protocol::MessageHeader header;
        std::string payload;
        
        if (!protocol::deserialize_message(request, header, payload)) {
            std::cerr << "Failed to deserialize message" << std::endl;
            return;
        }
        
        // Обработка различных команд
        switch (header.command) {
            case protocol::Command::GET_PROCESS_INFO:
                response = process_process_info_request();
                break;
            case protocol::Command::GET_THREAD_INFO:
                response = process_thread_info_request();
                break;
            default:
                std::cerr << "Unknown command: " << static_cast<int>(header.command) << std::endl;
                return;
        }
        
        // Логирование запроса
        std::string log_message = "Server2: Processed command " + 
                                 std::to_string(static_cast<int>(header.command)) + 
                                 " at " + utils::get_current_time_string();
        fifo_handler_->write_message(log_message);
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling message: " << e.what() << std::endl;
    }
}

std::string Server2::process_process_info_request() {
    try {
        protocol::ProcessInfo info;
        info.pid = utils::get_current_process_id();
        info.name = utils::get_process_name(info.pid);
        info.user = utils::get_process_user(info.pid);
        info.memory_usage = utils::get_process_memory_usage(info.pid);
        info.cpu_usage = utils::get_process_cpu_usage(info.pid);
        
        return serialization::serialize_process_info(info);
        
    } catch (const std::exception& e) {
        std::cerr << "Error processing process info: " << e.what() << std::endl;
        return "";
    }
}

std::string Server2::process_thread_info_request() {
    try {
        uint32_t pid = utils::get_current_process_id();
        auto thread_ids = utils::get_process_threads(pid);
        std::vector<protocol::ThreadInfo> thread_infos;
        
        for (uint32_t tid : thread_ids) {
            protocol::ThreadInfo info;
            info.tid = tid;
            info.name = utils::get_thread_name(tid);
            info.priority = utils::get_thread_priority(tid);
            info.state = utils::get_thread_state(tid);
            
            thread_infos.push_back(info);
        }
        
        return serialization::serialize_thread_info_vector(thread_infos);
        
    } catch (const std::exception& e) {
        std::cerr << "Error processing thread info: " << e.what() << std::endl;
        return "";
    }
}

} // namespace server2
} // namespace cs_app

