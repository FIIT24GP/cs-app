#include "fifo_handler.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace cs_app {
namespace logger {

FIFOHandler::FIFOHandler(const std::string& fifo_path) 
    : fifo_path_(fifo_path), fifo_fd_(-1), is_reader_(false), is_writer_(false) {
}

FIFOHandler::~FIFOHandler() {
    close();
}

bool FIFOHandler::create_fifo() {
    // Создание именованного канала
    if (mkfifo(fifo_path_.c_str(), 0666) == -1) {
        if (errno != EEXIST) {
            std::cerr << "Failed to create FIFO: " << strerror(errno) << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FIFOHandler::open_for_reading() {
    if (is_reader_) {
        return true;
    }
    
    fifo_fd_ = open(fifo_path_.c_str(), O_RDONLY | O_NONBLOCK);
    if (fifo_fd_ == -1) {
        std::cerr << "Failed to open FIFO for reading: " << strerror(errno) << std::endl;
        return false;
    }
    
    is_reader_ = true;
    return true;
}

bool FIFOHandler::open_for_writing() {
    if (is_writer_) {
        return true;
    }
    
    fifo_fd_ = open(fifo_path_.c_str(), O_WRONLY | O_NONBLOCK);
    if (fifo_fd_ == -1) {
        std::cerr << "Failed to open FIFO for writing: " << strerror(errno) << std::endl;
        return false;
    }
    
    is_writer_ = true;
    return true;
}

bool FIFOHandler::write_message(const std::string& message) {
    if (!is_writer_) {
        std::cerr << "FIFO not opened for writing" << std::endl;
        return false;
    }
    
    ssize_t bytes_written = write(fifo_fd_, message.c_str(), message.length());
    if (bytes_written == -1) {
        std::cerr << "Failed to write to FIFO: " << strerror(errno) << std::endl;
        return false;
    }
    
    return bytes_written == static_cast<ssize_t>(message.length());
}

bool FIFOHandler::read_message(std::string& message) {
    if (!is_reader_) {
        std::cerr << "FIFO not opened for reading" << std::endl;
        return false;
    }
    
    char buffer[1024];
    ssize_t bytes_read = read(fifo_fd_, buffer, sizeof(buffer) - 1);
    
    if (bytes_read == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Нет данных для чтения
            return false;
        }
        std::cerr << "Failed to read from FIFO: " << strerror(errno) << std::endl;
        return false;
    }
    
    if (bytes_read == 0) {
        // Конец файла
        return false;
    }
    
    buffer[bytes_read] = '\0';
    message = std::string(buffer);
    return true;
}

void FIFOHandler::close() {
    if (fifo_fd_ != -1) {
        ::close(fifo_fd_);
        fifo_fd_ = -1;
    }
    
    is_reader_ = false;
    is_writer_ = false;
}

} // namespace logger
} // namespace cs_app
