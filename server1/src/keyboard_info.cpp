#include "keyboard_info.h"
#include <iostream>
#include <cstdlib>

namespace cs_app {
namespace server1 {

protocol::KeyboardInfo KeyboardInfo::getKeyboardInfo() {
    protocol::KeyboardInfo info;
    
    try {
        // Получение информации о клавиатуре из переменных окружения
        const char* layout = std::getenv("XKB_DEFAULT_LAYOUT");
        const char* variant = std::getenv("XKB_DEFAULT_VARIANT");
        const char* model = std::getenv("XKB_DEFAULT_MODEL");
        
        info.layout = layout ? layout : "us";
        info.variant = variant ? variant : "";
        info.model = model ? model : "pc105";
        
    } catch (const std::exception& e) {
        std::cerr << "Error getting keyboard info: " << e.what() << std::endl;
        info.layout = "us";
        info.variant = "";
        info.model = "pc105";
    }
    
    return info;
}

} // namespace server1
} // namespace cs_app
