#include "serialization.h"
#include <sstream>
#include <cstring>

namespace cs_app {
namespace serialization {

std::string serialize_string(const std::string& str) {
    std::string result;
    uint32_t length = static_cast<uint32_t>(str.length());
    
    // Добавляем длину строки
    result.append(reinterpret_cast<const char*>(&length), sizeof(length));
    
    // Добавляем саму строку
    result.append(str);
    
    return result;
}

std::string deserialize_string(const std::string& data, size_t& offset) {
    if (offset + sizeof(uint32_t) > data.size()) {
        throw std::runtime_error("Insufficient data for string deserialization");
    }
    
    // Читаем длину строки
    uint32_t length;
    std::memcpy(&length, data.data() + offset, sizeof(length));
    offset += sizeof(length);
    
    // Проверяем, что данных достаточно
    if (offset + length > data.size()) {
        throw std::runtime_error("Insufficient data for string content");
    }
    
    // Извлекаем строку
    std::string result(data.data() + offset, length);
    offset += length;
    
    return result;
}

std::string serialize_disk_info(const protocol::DiskInfo& info) {
    std::string result;
    
    result += serialize_string(info.device_name);
    result += serialize_string(info.mount_point);
    result += serialize_string(info.filesystem_type);
    
    // Сериализация числовых значений
    result.append(reinterpret_cast<const char*>(&info.total_size), sizeof(info.total_size));
    result.append(reinterpret_cast<const char*>(&info.free_size), sizeof(info.free_size));
    
    return result;
}

protocol::DiskInfo deserialize_disk_info(const std::string& data) {
    protocol::DiskInfo info;
    size_t offset = 0;
    
    info.device_name = deserialize_string(data, offset);
    info.mount_point = deserialize_string(data, offset);
    info.filesystem_type = deserialize_string(data, offset);
    
    // Десериализация числовых значений
    std::memcpy(&info.total_size, data.data() + offset, sizeof(info.total_size));
    offset += sizeof(info.total_size);
    std::memcpy(&info.free_size, data.data() + offset, sizeof(info.free_size));
    
    return info;
}

std::string serialize_filesystem_info(const protocol::FilesystemInfo& info) {
    std::string result;
    
    result += serialize_string(info.type);
    result += serialize_string(info.mount_point);
    
    // Сериализация числовых значений
    result.append(reinterpret_cast<const char*>(&info.block_size), sizeof(info.block_size));
    result.append(reinterpret_cast<const char*>(&info.total_blocks), sizeof(info.total_blocks));
    result.append(reinterpret_cast<const char*>(&info.free_blocks), sizeof(info.free_blocks));
    
    return result;
}

protocol::FilesystemInfo deserialize_filesystem_info(const std::string& data) {
    protocol::FilesystemInfo info;
    size_t offset = 0;
    
    info.type = deserialize_string(data, offset);
    info.mount_point = deserialize_string(data, offset);
    
    // Десериализация числовых значений
    std::memcpy(&info.block_size, data.data() + offset, sizeof(info.block_size));
    offset += sizeof(info.block_size);
    std::memcpy(&info.total_blocks, data.data() + offset, sizeof(info.total_blocks));
    offset += sizeof(info.total_blocks);
    std::memcpy(&info.free_blocks, data.data() + offset, sizeof(info.free_blocks));
    
    return info;
}

std::string serialize_keyboard_info(const protocol::KeyboardInfo& info) {
    std::string result;
    
    result += serialize_string(info.layout);
    result += serialize_string(info.variant);
    result += serialize_string(info.model);
    
    return result;
}

protocol::KeyboardInfo deserialize_keyboard_info(const std::string& data) {
    protocol::KeyboardInfo info;
    size_t offset = 0;
    
    info.layout = deserialize_string(data, offset);
    info.variant = deserialize_string(data, offset);
    info.model = deserialize_string(data, offset);
    
    return info;
}

std::string serialize_process_info(const protocol::ProcessInfo& info) {
    std::string result;
    
    result += serialize_string(info.name);
    result += serialize_string(info.user);
    
    // Сериализация числовых значений
    result.append(reinterpret_cast<const char*>(&info.pid), sizeof(info.pid));
    result.append(reinterpret_cast<const char*>(&info.memory_usage), sizeof(info.memory_usage));
    result.append(reinterpret_cast<const char*>(&info.cpu_usage), sizeof(info.cpu_usage));
    
    return result;
}

protocol::ProcessInfo deserialize_process_info(const std::string& data) {
    protocol::ProcessInfo info;
    size_t offset = 0;
    
    info.name = deserialize_string(data, offset);
    info.user = deserialize_string(data, offset);
    
    // Десериализация числовых значений
    std::memcpy(&info.pid, data.data() + offset, sizeof(info.pid));
    offset += sizeof(info.pid);
    std::memcpy(&info.memory_usage, data.data() + offset, sizeof(info.memory_usage));
    offset += sizeof(info.memory_usage);
    std::memcpy(&info.cpu_usage, data.data() + offset, sizeof(info.cpu_usage));
    
    return info;
}

std::string serialize_thread_info(const protocol::ThreadInfo& info) {
    std::string result;
    
    result += serialize_string(info.name);
    result += serialize_string(info.state);
    
    // Сериализация числовых значений
    result.append(reinterpret_cast<const char*>(&info.tid), sizeof(info.tid));
    result.append(reinterpret_cast<const char*>(&info.priority), sizeof(info.priority));
    
    return result;
}

protocol::ThreadInfo deserialize_thread_info(const std::string& data) {
    protocol::ThreadInfo info;
    size_t offset = 0;
    
    info.name = deserialize_string(data, offset);
    info.state = deserialize_string(data, offset);
    
    // Десериализация числовых значений
    std::memcpy(&info.tid, data.data() + offset, sizeof(info.tid));
    offset += sizeof(info.tid);
    std::memcpy(&info.priority, data.data() + offset, sizeof(info.priority));
    
    return info;
}

std::string serialize_disk_info_vector(const std::vector<protocol::DiskInfo>& infos) {
    std::string result;
    
    // Сериализация количества элементов
    uint32_t count = static_cast<uint32_t>(infos.size());
    result.append(reinterpret_cast<const char*>(&count), sizeof(count));
    
    // Сериализация каждого элемента
    for (const auto& info : infos) {
        std::string serialized = serialize_disk_info(info);
        uint32_t length = static_cast<uint32_t>(serialized.length());
        result.append(reinterpret_cast<const char*>(&length), sizeof(length));
        result.append(serialized);
    }
    
    return result;
}

std::vector<protocol::DiskInfo> deserialize_disk_info_vector(const std::string& data) {
    std::vector<protocol::DiskInfo> result;
    size_t offset = 0;
    
    // Читаем количество элементов
    uint32_t count;
    std::memcpy(&count, data.data() + offset, sizeof(count));
    offset += sizeof(count);
    
    // Читаем каждый элемент
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t length;
        std::memcpy(&length, data.data() + offset, sizeof(length));
        offset += sizeof(length);
        
        std::string element_data(data.data() + offset, length);
        result.push_back(deserialize_disk_info(element_data));
        offset += length;
    }
    
    return result;
}

std::string serialize_thread_info_vector(const std::vector<protocol::ThreadInfo>& infos) {
    std::string result;
    
    // Сериализация количества элементов
    uint32_t count = static_cast<uint32_t>(infos.size());
    result.append(reinterpret_cast<const char*>(&count), sizeof(count));
    
    // Сериализация каждого элемента
    for (const auto& info : infos) {
        std::string serialized = serialize_thread_info(info);
        uint32_t length = static_cast<uint32_t>(serialized.length());
        result.append(reinterpret_cast<const char*>(&length), sizeof(length));
        result.append(serialized);
    }
    
    return result;
}

std::vector<protocol::ThreadInfo> deserialize_thread_info_vector(const std::string& data) {
    std::vector<protocol::ThreadInfo> result;
    size_t offset = 0;
    
    // Читаем количество элементов
    uint32_t count;
    std::memcpy(&count, data.data() + offset, sizeof(count));
    offset += sizeof(count);
    
    // Читаем каждый элемент
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t length;
        std::memcpy(&length, data.data() + offset, sizeof(length));
        offset += sizeof(length);
        
        std::string element_data(data.data() + offset, length);
        result.push_back(deserialize_thread_info(element_data));
        offset += length;
    }
    
    return result;
}

} // namespace serialization
} // namespace cs_app
