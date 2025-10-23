#pragma once

#include <vector>
#include "protocol.h"

namespace cs_app {
namespace server2 {

class ThreadInfo {
public:
    static std::vector<protocol::ThreadInfo> getThreadInfo();
};

} // namespace server2
} // namespace cs_app

