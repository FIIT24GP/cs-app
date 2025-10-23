#include "disk_info.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>
#include <filesystem>

namespace cs_app {
namespace server1 {

std::vector<protocol::DiskInfo> DiskInfo::getDiskInfo() {
    std::vector<protocol::DiskInfo> disk_infos;
    
    try {
        auto devices = getDiskDevices();
        
        for (const auto& device : devices) {
            protocol::DiskInfo info;
            info.device_name = device;
            info.mount_point = "/mnt/" + device; // Упрощенная реализация
            info.filesystem_type = getFilesystemType(device);
            info.total_size = getDiskSize(device);
            info.free_size = getDiskFreeSpace(device);
            
            disk_infos.push_back(info);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error getting disk info: " << e.what() << std::endl;
    }
    
    return disk_infos;
}

std::vector<std::string> DiskInfo::getDiskDevices() {
    std::vector<std::string> devices;
    
    try {
        // Чтение /proc/partitions для получения списка устройств
        std::ifstream file("/proc/partitions");
        std::string line;
        
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string major, minor, blocks, name;
            
            if (iss >> major >> minor >> blocks >> name) {
                if (name.find("sd") == 0 || name.find("hd") == 0) {
                    devices.push_back("/dev/" + name);
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error reading disk devices: " << e.what() << std::endl;
    }
    
    return devices;
}

std::string DiskInfo::getFilesystemType(const std::string& device) {
    try {
        // Упрощенная реализация - возвращаем ext4 по умолчанию
        return "ext4";
    } catch (const std::exception& e) {
        std::cerr << "Error getting filesystem type: " << e.what() << std::endl;
        return "unknown";
    }
}

uint64_t DiskInfo::getDiskSize(const std::string& device) {
    try {
        // Упрощенная реализация - возвращаем фиксированное значение
        return 1000000000; // 1GB
    } catch (const std::exception& e) {
        std::cerr << "Error getting disk size: " << e.what() << std::endl;
        return 0;
    }
}

uint64_t DiskInfo::getDiskFreeSpace(const std::string& device) {
    try {
        // Упрощенная реализация - возвращаем фиксированное значение
        return 500000000; // 500MB
    } catch (const std::exception& e) {
        std::cerr << "Error getting disk free space: " << e.what() << std::endl;
        return 0;
    }
}

} // namespace server1
} // namespace cs_app
