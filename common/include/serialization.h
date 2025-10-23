#pragma once

#include "protocol.h"
#include <string>
#include <vector>

namespace cs_app {
namespace serialization {

// Сериализация структур данных
std::string serialize_disk_info(const protocol::DiskInfo& info);
std::string serialize_filesystem_info(const protocol::FilesystemInfo& info);
std::string serialize_keyboard_info(const protocol::KeyboardInfo& info);
std::string serialize_process_info(const protocol::ProcessInfo& info);
std::string serialize_thread_info(const protocol::ThreadInfo& info);

// Десериализация структур данных
protocol::DiskInfo deserialize_disk_info(const std::string& data);
protocol::FilesystemInfo deserialize_filesystem_info(const std::string& data);
protocol::KeyboardInfo deserialize_keyboard_info(const std::string& data);
protocol::ProcessInfo deserialize_process_info(const std::string& data);
protocol::ThreadInfo deserialize_thread_info(const std::string& data);

// Сериализация векторов
std::string serialize_disk_info_vector(const std::vector<protocol::DiskInfo>& infos);
std::string serialize_thread_info_vector(const std::vector<protocol::ThreadInfo>& infos);

// Десериализация векторов
std::vector<protocol::DiskInfo> deserialize_disk_info_vector(const std::string& data);
std::vector<protocol::ThreadInfo> deserialize_thread_info_vector(const std::string& data);

// Утилиты для работы со строками
std::string serialize_string(const std::string& str);
std::string deserialize_string(const std::string& data, size_t& offset);

} // namespace serialization
} // namespace cs_app

