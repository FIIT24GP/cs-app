#include "filesystem_info.h"
#include <iostream>

namespace cs_app {
namespace server1 {

protocol::FilesystemInfo FilesystemInfo::getFilesystemInfo() {
    protocol::FilesystemInfo info;
    
    try {
        info.type = "ext4";
        info.mount_point = "/";
        info.block_size = 4096;
        info.total_blocks = 1000000;
        info.free_blocks = 500000;
        
    } catch (const std::exception& e) {
        std::cerr << "Error getting filesystem info: " << e.what() << std::endl;
    }
    
    return info;
}

} // namespace server1
} // namespace cs_app
