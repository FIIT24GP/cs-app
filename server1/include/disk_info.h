#pragma once

#include <string>
#include <vector>
#include "protocol.h"

namespace cs_app {
namespace server1 {

class DiskInfo {
public:
    static std::vector<protocol::DiskInfo> getDiskInfo();
    
private:
    static std::vector<std::string> getDiskDevices();
    static std::string getFilesystemType(const std::string& device);
    static uint64_t getDiskSize(const std::string& device);
    static uint64_t getDiskFreeSpace(const std::string& device);
};

} // namespace server1
} // namespace cs_app
