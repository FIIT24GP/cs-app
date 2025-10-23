#pragma once

#include "protocol.h"

namespace cs_app {
namespace server1 {

class KeyboardInfo {
public:
    static protocol::KeyboardInfo getKeyboardInfo();
};

} // namespace server1
} // namespace cs_app

