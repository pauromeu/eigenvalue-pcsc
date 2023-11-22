#include <gtest/gtest.h>
#include "Greeter.h"

TEST(GreetingTest, BasicTest)
{
    EXPECT_EQ(Greeter("World").sayHello(), "Hello, World!");
}
