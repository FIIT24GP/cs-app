#pragma once

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include "fifo_handler.h"
#include "log_writer.h"

namespace cs_app {
namespace logger {

class Logger {
public:
    Logger();
    ~Logger();
    
    bool start();
    void stop();
    void wait();
    
private:
    void process_logs();
    void process_server1_logs();
    void process_server2_logs();
    
    std::unique_ptr<FIFOHandler> server1_fifo_;
    std::unique_ptr<FIFOHandler> server2_fifo_;
    std::unique_ptr<LogWriter> log_writer_;
    
    std::thread server1_thread_;
    std::thread server2_thread_;
    std::atomic<bool> running_;
};

} // namespace logger
} // namespace cs_app

