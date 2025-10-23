#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace cs_app {
namespace utils {

// Утилиты для работы со временем
uint64_t get_current_timestamp();
std::string format_timestamp(uint64_t timestamp);
std::string get_current_time_string();

// Утилиты для работы с файловой системой
std::vector<std::string> get_disk_devices();
std::string get_filesystem_type(const std::string& device);
uint64_t get_disk_size(const std::string& device);
uint64_t get_disk_free_space(const std::string& device);

// Утилиты для работы с процессами
uint32_t get_current_process_id();
std::string get_process_name(uint32_t pid);
std::string get_process_user(uint32_t pid);
uint64_t get_process_memory_usage(uint32_t pid);
double get_process_cpu_usage(uint32_t pid);

// Утилиты для работы с потоками
std::vector<uint32_t> get_process_threads(uint32_t pid);
std::string get_thread_name(uint32_t tid);
uint32_t get_thread_priority(uint32_t tid);
std::string get_thread_state(uint32_t tid);

// Утилиты для работы с клавиатурой
std::string get_keyboard_layout();
std::string get_keyboard_variant();
std::string get_keyboard_model();

// Утилиты для логирования
void log_info(const std::string& message);
void log_warning(const std::string& message);
void log_error(const std::string& message);
void log_debug(const std::string& message);

// Утилиты для работы со строками
std::string trim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

// Утилиты для работы с сетью
bool is_port_available(uint16_t port);
std::string get_local_ip_address();

} // namespace utils
} // namespace cs_app
