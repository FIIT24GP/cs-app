#include "thread_info.h"
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <thread>

namespace cs_app {
namespace server2 {

std::vector<protocol::ThreadInfo> ThreadInfo::getThreadInfo() {
    std::vector<protocol::ThreadInfo> thread_infos;
    
    try {
        // Получение информации о текущем потоке
        protocol::ThreadInfo main_thread;
        main_thread.tid = syscall(SYS_gettid);
        main_thread.name = "main_thread";
        main_thread.priority = 0;
        main_thread.state = "running";
        
        thread_infos.push_back(main_thread);
        
        // В реальной реализации здесь должно быть получение информации о всех потоках процесса
        
    } catch (const std::exception& e) {
        std::cerr << "Error getting thread info: " << e.what() << std::endl;
    }
    
    return thread_infos;
}

} // namespace server2
} // namespace cs_app

