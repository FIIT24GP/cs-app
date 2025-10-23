#include <gtest/gtest.h>
#include "serialization.h"

using namespace cs_app;

TEST(SerializationTest, DiskInfoSerialization) {
    protocol::DiskInfo info;
    info.device_name = "/dev/sda1";
    info.mount_point = "/";
    info.filesystem_type = "ext4";
    info.total_size = 1000000000;
    info.free_size = 500000000;
    
    std::string serialized = serialization::serialize_disk_info(info);
    protocol::DiskInfo deserialized = serialization::deserialize_disk_info(serialized);
    
    EXPECT_EQ(info.device_name, deserialized.device_name);
    EXPECT_EQ(info.mount_point, deserialized.mount_point);
    EXPECT_EQ(info.filesystem_type, deserialized.filesystem_type);
    EXPECT_EQ(info.total_size, deserialized.total_size);
    EXPECT_EQ(info.free_size, deserialized.free_size);
}

TEST(SerializationTest, ProcessInfoSerialization) {
    protocol::ProcessInfo info;
    info.pid = 1234;
    info.name = "test_process";
    info.user = "test_user";
    info.memory_usage = 1024000;
    info.cpu_usage = 15.5;
    
    std::string serialized = serialization::serialize_process_info(info);
    protocol::ProcessInfo deserialized = serialization::deserialize_process_info(serialized);
    
    EXPECT_EQ(info.pid, deserialized.pid);
    EXPECT_EQ(info.name, deserialized.name);
    EXPECT_EQ(info.user, deserialized.user);
    EXPECT_EQ(info.memory_usage, deserialized.memory_usage);
    EXPECT_DOUBLE_EQ(info.cpu_usage, deserialized.cpu_usage);
}

