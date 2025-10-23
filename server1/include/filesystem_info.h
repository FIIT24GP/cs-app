#pragma once

#include "protocol.h"

namespace cs_app {
namespace server1 {

class FilesystemInfo {
public:
    static protocol::FilesystemInfo getFilesystemInfo();
};

} // namespace server1
} // namespace cs_app
