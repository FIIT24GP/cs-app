#pragma once

#include <memory>
#include <string>
#include "network.h"
#include "protocol.h"
#include "serialization.h"

namespace cs_app {
namespace server1 {

class Server1 {
public:
    Server1();
    ~Server1();
    
    bool start();
    void stop();
    void wait();
    
private:
    void handle_message(const std::string& request, std::string& response);
    std::string process_disk_info_request();
    std::string process_filesystem_info_request();
    std::string process_keyboard_info_request();
    
    std::unique_ptr<network::TCPServer> tcp_server_;
    std::unique_ptr<network::FIFOHandler> fifo_handler_;
    bool running_;
};

} // namespace server1
} // namespace cs_app
