#include "utils.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/statvfs.h>
#include <iostream>
#include <cstring>

namespace cs_app {
namespace utils {

uint64_t get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

std::string format_timestamp(uint64_t timestamp) {
    auto time_point = std::chrono::system_clock::time_point(std::chrono::milliseconds(timestamp));
    auto time_t = std::chrono::system_clock::to_time_t(time_point);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string get_current_time_string() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::vector<std::string> get_disk_devices() {
    std::vector<std::string> devices;
    
    try {
        std::ifstream file("/proc/partitions");
        std::string line;
        
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string major, minor, blocks, name;
            
            if (iss >> major >> minor >> blocks >> name) {
                if (name.find("sd") == 0 || name.find("hd") == 0) {
                    devices.push_back("/dev/" + name);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading disk devices: " << e.what() << std::endl;
    }
    
    return devices;
}

std::string get_filesystem_type(const std::string& device) {
    // Упрощенная реализация
    return "ext4";
}

uint64_t get_disk_size(const std::string& device) {
    // Упрощенная реализация
    return 1000000000; // 1GB
}

uint64_t get_disk_free_space(const std::string& device) {
    // Упрощенная реализация
    return 500000000; // 500MB
}

uint32_t get_current_process_id() {
    return getpid();
}

std::string get_process_name(uint32_t pid) {
    if (pid == getpid()) {
        return "current_process";
    }
    return "unknown_process";
}

std::string get_process_user(uint32_t pid) {
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return "unknown_user";
}

uint64_t get_process_memory_usage(uint32_t pid) {
    // Упрощенная реализация
    return 1024000; // 1MB
}

double get_process_cpu_usage(uint32_t pid) {
    // Упрощенная реализация
    return 0.5; // 0.5%
}

std::vector<uint32_t> get_process_threads(uint32_t pid) {
    std::vector<uint32_t> threads;
    
    try {
        // Получение ID текущего потока
        threads.push_back(syscall(SYS_gettid));
    } catch (const std::exception& e) {
        std::cerr << "Error getting process threads: " << e.what() << std::endl;
    }
    
    return threads;
}

std::string get_thread_name(uint32_t tid) {
    return "thread_" + std::to_string(tid);
}

uint32_t get_thread_priority(uint32_t tid) {
    return 0;
}

std::string get_thread_state(uint32_t tid) {
    return "running";
}

std::string get_keyboard_layout() {
    const char* layout = std::getenv("XKB_DEFAULT_LAYOUT");
    return layout ? layout : "us";
}

std::string get_keyboard_variant() {
    const char* variant = std::getenv("XKB_DEFAULT_VARIANT");
    return variant ? variant : "";
}

std::string get_keyboard_model() {
    const char* model = std::getenv("XKB_DEFAULT_MODEL");
    return model ? model : "pc105";
}

void log_info(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void log_warning(const std::string& message) {
    std::cout << "[WARNING] " << message << std::endl;
}

void log_error(const std::string& message) {
    std::cerr << "[ERROR] " << message << std::endl;
}

void log_debug(const std::string& message) {
    std::cout << "[DEBUG] " << message << std::endl;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) {
        return "";
    }
    
    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }
    
    return result;
}

bool is_port_available(uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    bool available = bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0;
    close(sock);
    
    return available;
}

std::string get_local_ip_address() {
    // Упрощенная реализация
    return "127.0.0.1";
}

} // namespace utils
} // namespace cs_app
