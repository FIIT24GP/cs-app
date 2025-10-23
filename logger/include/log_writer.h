#pragma once

#include <string>
#include <fstream>
#include <mutex>

namespace cs_app {
namespace logger {

class LogWriter {
public:
    LogWriter();
    ~LogWriter();
    
    void write_log(const std::string& source, const std::string& message);
    
private:
    std::string get_log_filename(const std::string& source);
    std::string format_log_message(const std::string& source, const std::string& message);
    
    std::mutex log_mutex_;
    std::string log_directory_;
};

} // namespace logger
} // namespace cs_app
