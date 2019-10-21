// Currently we have to include the .cpp files or change DemoServer's configuration type 
// to DLL. We should probably look into another solution later.
#include "gtest/gtest.h"
#include <stdint.h>
#include <iostream>

#include "../src/Utils/AckBuffer.cpp"


TEST(ExampleTest, exampleTestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(AckBufferTest, emptyOnCreation) {
	AckBuffer ackBuffer = AckBuffer();
	ASSERT_TRUE(ackBuffer.empty);
}

TEST(AckBufferTest, acceptsSequences) {
	AckBuffer ackBuffer = AckBuffer();
	
	for (int i = 0; i < 25; ++i) {
		int randSequence = rand() % 65536;
		ackBuffer.put(randSequence);
	}

	ASSERT_FALSE(ackBuffer.empty);
}

TEST(AckBufferTest, acceptsRandomSequences) {
	AckBuffer ackBuffer = AckBuffer();

	int start = rand() % 65000;
	for (int j = start; j < start + 50; ++j) {
		bool fill = rand() % 2 == 1;
		if (fill) {
			ASSERT_TRUE(ackBuffer.put(j));
			ASSERT_EQ(ackBuffer.last_sequence, j);
		}
	}
		
	ASSERT_FALSE(ackBuffer.empty);
}

TEST(AckBufferTest, acceptsSmallerSequences) {
	AckBuffer ackBuffer = AckBuffer();
	
	ASSERT_TRUE(ackBuffer.put(5));
	ASSERT_TRUE(ackBuffer.put(6));
	ASSERT_FALSE(ackBuffer.put(4));
	ASSERT_TRUE(ackBuffer.put(10));
	ASSERT_FALSE(ackBuffer.put(7));
	ASSERT_FALSE(ackBuffer.put(8));
	ASSERT_TRUE(ackBuffer.put(5000));
	ASSERT_FALSE(ackBuffer.put(9));
}

TEST(AckBufferTest, acceptsOverflowingSequences) {
	AckBuffer ackBuffer = AckBuffer();

	ASSERT_TRUE(ackBuffer.put(65534));
	ASSERT_TRUE(ackBuffer.put(65535));
	ASSERT_TRUE(ackBuffer.put(0));
	ASSERT_TRUE(ackBuffer.put(1));
}

TEST(AckBufferTest, fullBitfieldWhenAcksSequential) {
	AckBuffer ackBuffer = AckBuffer();

	for (int i = 0; i < 33; ++i) {
		ASSERT_TRUE(ackBuffer.put(i));
	}

	EXPECT_EQ(ackBuffer.last_sequence, 32);
	EXPECT_EQ(ackBuffer.ack_bitfield, 0xffffffff);
}
