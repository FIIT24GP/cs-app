#include <gtest/gtest.h>
#include "protocol.h"
#include "serialization.h"
#include "utils.h"

using namespace cs_app;

// Тесты для протокола
TEST(ProtocolTest, MessageHeaderValidation) {
    protocol::MessageHeader header;
    header.magic = 0xDEADBEEF;
    header.length = 10;
    
    EXPECT_TRUE(protocol::validate_header(header));
    
    header.magic = 0x12345678;
    EXPECT_FALSE(protocol::validate_header(header));
    
    header.magic = 0xDEADBEEF;
    header.length = 0;
    EXPECT_FALSE(protocol::validate_header(header));
}

TEST(ProtocolTest, MessageSerialization) {
    protocol::MessageHeader header;
    header.magic = 0xDEADBEEF;
    header.length = 5;
    header.type = protocol::MessageType::REQUEST;
    header.command = protocol::Command::GET_DISK_INFO;
    header.request_id = 123;
    header.timestamp = 1234567890;
    
    std::string data = "hello";
    std::string serialized = protocol::serialize_message(header, data);
    
    protocol::MessageHeader deserialized_header;
    std::string deserialized_data;
    
    EXPECT_TRUE(protocol::deserialize_message(serialized, deserialized_header, deserialized_data));
    EXPECT_EQ(header.magic, deserialized_header.magic);
    EXPECT_EQ(header.length, deserialized_header.length);
    EXPECT_EQ(header.type, deserialized_header.type);
    EXPECT_EQ(header.command, deserialized_header.command);
    EXPECT_EQ(header.request_id, deserialized_header.request_id);
    EXPECT_EQ(header.timestamp, deserialized_header.timestamp);
    EXPECT_EQ(data, deserialized_data);
}

// Тесты для сериализации
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

// Тесты для утилит
TEST(UtilsTest, StringUtilities) {
    std::string test_string = "  hello world  ";
    std::string trimmed = utils::trim(test_string);
    EXPECT_EQ("hello world", trimmed);
    
    std::string test_split = "a,b,c,d";
    auto parts = utils::split(test_split, ',');
    EXPECT_EQ(4, parts.size());
    EXPECT_EQ("a", parts[0]);
    EXPECT_EQ("b", parts[1]);
    EXPECT_EQ("c", parts[2]);
    EXPECT_EQ("d", parts[3]);
    
    std::vector<std::string> test_join = {"a", "b", "c"};
    std::string joined = utils::join(test_join, "-");
    EXPECT_EQ("a-b-c", joined);
}

TEST(UtilsTest, TimestampUtilities) {
    uint64_t timestamp = utils::get_current_timestamp();
    EXPECT_GT(timestamp, 0);
    
    std::string time_string = utils::get_current_time_string();
    EXPECT_FALSE(time_string.empty());
    
    std::string formatted = utils::format_timestamp(timestamp);
    EXPECT_FALSE(formatted.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
