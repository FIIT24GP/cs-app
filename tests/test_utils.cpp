#include <gtest/gtest.h>
#include "utils.h"

using namespace cs_app;

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

