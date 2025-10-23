#pragma once

#include <memory>
#include <string>
#include <vector>
#include "protocol.h"
#include "serialization.h"
#include "network.h"

namespace cs_app {
namespace client {

class Client {
public:
    Client();
    ~Client();
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    // Методы для работы с Server1
    std::vector<protocol::DiskInfo> getDiskInfo();
    protocol::FilesystemInfo getFilesystemInfo();
    protocol::KeyboardInfo getKeyboardInfo();
    
    // Методы для работы с Server2
    protocol::ProcessInfo getProcessInfo();
    std::vector<protocol::ThreadInfo> getThreadInfo();
    
private:
    bool sendRequest(protocol::Command command, const std::string& data);
    std::string receiveResponse();
    
    std::unique_ptr<network::TCPClient> server1_client_;
    std::unique_ptr<network::TCPClient> server2_client_;
    bool connected_;
};

} // namespace client
} // namespace cs_app

