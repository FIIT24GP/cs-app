#include <iostream>
#include <csignal>
#include <memory>
#include "logger.h"
#include "utils.h"

std::unique_ptr<cs_app::logger::Logger> g_logger;

void signal_handler(int signal) {
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    if (g_logger) {
        g_logger->stop();
    }
}

int main() {
    // Установка обработчика сигналов
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    try {
        // Создание и запуск логгера
        g_logger = std::make_unique<cs_app::logger::Logger>();
        
        if (!g_logger->start()) {
            std::cerr << "Failed to start logger" << std::endl;
            return 1;
        }
        
        std::cout << "Logger started successfully" << std::endl;
        std::cout << "Press Ctrl+C to stop the logger" << std::endl;
        
        // Ожидание завершения
        g_logger->wait();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
