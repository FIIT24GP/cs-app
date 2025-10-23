#pragma once

#include "protocol.h"

namespace cs_app {
namespace server2 {

class ProcessInfo {
public:
    static protocol::ProcessInfo getProcessInfo();
};

} // namespace server2
} // namespace cs_app
