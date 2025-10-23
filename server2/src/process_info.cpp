#include "process_info.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

namespace cs_app {
namespace server2 {

protocol::ProcessInfo ProcessInfo::getProcessInfo() {
    protocol::ProcessInfo info;
    
    try {
        info.pid = getpid();
        info.name = "server2";
        info.user = "server_user";
        info.memory_usage = 1024000; // 1MB
        info.cpu_usage = 0.5; // 0.5%
        
    } catch (const std::exception& e) {
        std::cerr << "Error getting process info: " << e.what() << std::endl;
    }
    
    return info;
}

} // namespace server2
} // namespace cs_app
