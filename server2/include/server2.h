#pragma once

#include <memory>
#include <string>
#include "network.h"
#include "protocol.h"
#include "serialization.h"

namespace cs_app {
namespace server2 {

class Server2 {
public:
    Server2();
    ~Server2();
    
    bool start();
    void stop();
    void wait();
    
private:
    void handle_message(const std::string& request, std::string& response);
    std::string process_process_info_request();
    std::string process_thread_info_request();
    
    std::unique_ptr<network::TCPServer> tcp_server_;
    std::unique_ptr<network::FIFOHandler> fifo_handler_;
    bool running_;
};

} // namespace server2
} // namespace cs_app
