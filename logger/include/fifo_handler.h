#pragma once

#include <string>

namespace cs_app {
namespace logger {

class FIFOHandler {
public:
    FIFOHandler(const std::string& fifo_path);
    ~FIFOHandler();
    
    bool create_fifo();
    bool open_for_reading();
    bool open_for_writing();
    bool write_message(const std::string& message);
    bool read_message(std::string& message);
    void close();
    
private:
    std::string fifo_path_;
    int fifo_fd_;
    bool is_reader_;
    bool is_writer_;
};

} // namespace logger
} // namespace cs_app
