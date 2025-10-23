#include <gtest/gtest.h>
#include "protocol.h"

using namespace cs_app;

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

