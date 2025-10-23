#include <iostream>
#include <csignal>
#include <memory>
#include "server1.h"
#include "utils.h"

std::unique_ptr<cs_app::server1::Server1> g_server;

void signal_handler(int signal) {
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    if (g_server) {
        g_server->stop();
    }
}

int main() {
    // Установка обработчика сигналов
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    try {
        // Создание и запуск сервера
        g_server = std::make_unique<cs_app::server1::Server1>();
        
        if (!g_server->start()) {
            std::cerr << "Failed to start server1" << std::endl;
            return 1;
        }
        
        std::cout << "Server1 started successfully on port 8081" << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        
        // Ожидание завершения
        g_server->wait();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
