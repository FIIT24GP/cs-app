#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace cs_app {
namespace protocol {

// Типы сообщений
enum class MessageType : uint8_t {
    REQUEST = 0x01,
    RESPONSE = 0x02,
    LOG = 0x03,
    ERROR = 0x04
};

// Коды команд для серверов
enum class Command : uint8_t {
    GET_DISK_INFO = 0x10,
    GET_FILESYSTEM_INFO = 0x11,
    GET_KEYBOARD_INFO = 0x12,
    GET_PROCESS_INFO = 0x20,
    GET_THREAD_INFO = 0x21
};

// Структура заголовка сообщения
struct MessageHeader {
    uint32_t magic;           // Магическое число для проверки целостности
    uint32_t length;          // Длина данных
    MessageType type;         // Тип сообщения
    Command command;          // Команда
    uint32_t request_id;      // ID запроса для сопоставления ответов
    uint64_t timestamp;       // Временная метка
    
    MessageHeader() : magic(0xDEADBEEF), length(0), type(MessageType::REQUEST), 
                     command(Command::GET_DISK_INFO), request_id(0), timestamp(0) {}
};

// Структура для информации о диске
struct DiskInfo {
    std::string device_name;
    std::string mount_point;
    uint64_t total_size;
    uint64_t free_size;
    std::string filesystem_type;
    
    DiskInfo() : total_size(0), free_size(0) {}
};

// Структура для информации о файловой системе
struct FilesystemInfo {
    std::string type;
    std::string mount_point;
    uint64_t block_size;
    uint64_t total_blocks;
    uint64_t free_blocks;
    
    FilesystemInfo() : block_size(0), total_blocks(0), free_blocks(0) {}
};

// Структура для информации о клавиатуре
struct KeyboardInfo {
    std::string layout;
    std::string variant;
    std::string model;
    
    KeyboardInfo() = default;
};

// Структура для информации о процессе
struct ProcessInfo {
    uint32_t pid;
    std::string name;
    std::string user;
    uint64_t memory_usage;
    double cpu_usage;
    
    ProcessInfo() : pid(0), memory_usage(0), cpu_usage(0.0) {}
};

// Структура для информации о потоке
struct ThreadInfo {
    uint32_t tid;
    std::string name;
    uint32_t priority;
    std::string state;
    
    ThreadInfo() : tid(0), priority(0) {}
};

// Функции для работы с протоколом
bool validate_header(const MessageHeader& header);
std::string serialize_message(const MessageHeader& header, const std::string& data);
bool deserialize_message(const std::string& data, MessageHeader& header, std::string& payload);

} // namespace protocol
} // namespace cs_app

