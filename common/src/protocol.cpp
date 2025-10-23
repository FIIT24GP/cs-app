#include "protocol.h"
#include <cstring>
#include <stdexcept>

namespace cs_app {
namespace protocol {

bool validate_header(const MessageHeader& header) {
    return header.magic == 0xDEADBEEF && header.length > 0;
}

std::string serialize_message(const MessageHeader& header, const std::string& data) {
    std::string result;
    result.reserve(sizeof(MessageHeader) + data.size());
    
    // Сериализация заголовка
    result.append(reinterpret_cast<const char*>(&header), sizeof(MessageHeader));
    
    // Добавление данных
    result.append(data);
    
    return result;
}

bool deserialize_message(const std::string& data, MessageHeader& header, std::string& payload) {
    if (data.size() < sizeof(MessageHeader)) {
        return false;
    }
    
    // Десериализация заголовка
    std::memcpy(&header, data.data(), sizeof(MessageHeader));
    
    // Проверка валидности заголовка
    if (!validate_header(header)) {
        return false;
    }
    
    // Проверка размера данных
    if (data.size() < sizeof(MessageHeader) + header.length) {
        return false;
    }
    
    // Извлечение полезной нагрузки
    payload = data.substr(sizeof(MessageHeader), header.length);
    
    return true;
}

} // namespace protocol
} // namespace cs_app
