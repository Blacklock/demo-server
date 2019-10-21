#include "gtest/gtest.h"
#include "../src/Utils/AckBuffer.h"

TEST(ExampleTest, exampleTestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(AckBufferTest, isEmpty) {
	AckBuffer ackBuffer = AckBuffer();
	//EXPECT_TRUE(ackBuffer.empty);
}
