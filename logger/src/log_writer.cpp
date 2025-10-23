#include "log_writer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <filesystem>

namespace cs_app {
namespace logger {

LogWriter::LogWriter() : log_directory_("/tmp/logs") {
    // Создание директории для логов
    std::filesystem::create_directories(log_directory_);
}

LogWriter::~LogWriter() {
    // Деструктор
}

void LogWriter::write_log(const std::string& source, const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    
    std::string filename = get_log_filename(source);
    std::string formatted_message = format_log_message(source, message);
    
    std::ofstream log_file(filename, std::ios::app);
    if (log_file.is_open()) {
        log_file << formatted_message << std::endl;
        log_file.close();
    } else {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

std::string LogWriter::get_log_filename(const std::string& source) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << log_directory_ << "/" << source << "_" 
        << std::put_time(&tm, "%Y%m%d") << ".log";
    
    return oss.str();
}

std::string LogWriter::format_log_message(const std::string& source, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << source << "] " << message;
    
    return oss.str();
}

} // namespace logger
} // namespace cs_app

