#include <gtest/gtest.h>
#include "greetings.h"

TEST(GreetingTest, BasicTest)
{
    EXPECT_EQ(getGreeting("World"), "Hello, World!");
}
