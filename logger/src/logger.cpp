#include "logger.h"
#include <iostream>
#include <chrono>

namespace cs_app {
namespace logger {

Logger::Logger() : running_(false) {
    server1_fifo_ = std::make_unique<FIFOHandler>("/tmp/server1.log.fifo");
    server2_fifo_ = std::make_unique<FIFOHandler>("/tmp/server2.log.fifo");
    log_writer_ = std::make_unique<LogWriter>();
}

Logger::~Logger() {
    stop();
}

bool Logger::start() {
    if (running_) {
        return true;
    }
    
    // Создание именованных каналов
    if (!server1_fifo_->create_fifo()) {
        std::cerr << "Failed to create server1 FIFO" << std::endl;
        return false;
    }
    
    if (!server2_fifo_->create_fifo()) {
        std::cerr << "Failed to create server2 FIFO" << std::endl;
        return false;
    }
    
    // Открытие каналов для чтения
    if (!server1_fifo_->open_for_reading()) {
        std::cerr << "Failed to open server1 FIFO for reading" << std::endl;
        return false;
    }
    
    if (!server2_fifo_->open_for_reading()) {
        std::cerr << "Failed to open server2 FIFO for reading" << std::endl;
        return false;
    }
    
    // Запуск потоков обработки логов
    running_ = true;
    server1_thread_ = std::thread(&Logger::process_server1_logs, this);
    server2_thread_ = std::thread(&Logger::process_server2_logs, this);
    
    return true;
}

void Logger::stop() {
    if (!running_) {
        return;
    }
    
    running_ = false;
    
    // Ожидание завершения потоков
    if (server1_thread_.joinable()) {
        server1_thread_.join();
    }
    
    if (server2_thread_.joinable()) {
        server2_thread_.join();
    }
    
    // Закрытие каналов
    server1_fifo_->close();
    server2_fifo_->close();
}

void Logger::wait() {
    if (server1_thread_.joinable()) {
        server1_thread_.join();
    }
    
    if (server2_thread_.joinable()) {
        server2_thread_.join();
    }
}

void Logger::process_server1_logs() {
    std::string message;
    
    while (running_) {
        if (server1_fifo_->read_message(message)) {
            log_writer_->write_log("server1", message);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void Logger::process_server2_logs() {
    std::string message;
    
    while (running_) {
        if (server2_fifo_->read_message(message)) {
            log_writer_->write_log("server2", message);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

} // namespace logger
} // namespace cs_app
