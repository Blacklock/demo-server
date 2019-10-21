#pragma once

#include "gtest/gtest.h"
#include <stdint.h>

#include "../src/Utils/AckBuffer.cpp"


class AckBufferTest : public ::testing::Test {
protected:
	void SetUp() override {
		ackBuffer = AckBuffer();
	}

	AckBuffer ackBuffer;
};

TEST_F(AckBufferTest, emptyOnCreation) {
	ASSERT_TRUE(ackBuffer.empty);
}

TEST_F(AckBufferTest, handlesRandomSequences) {
	for (int i = 0; i < 25; ++i) {
		int randSequence = rand() % 65536;
		ackBuffer.put(randSequence);
	}

	ASSERT_FALSE(ackBuffer.empty);
}

TEST_F(AckBufferTest, handlesSequencesWithGaps) {
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

TEST_F(AckBufferTest, handlesDecreasingSequences) {
	ASSERT_TRUE(ackBuffer.put(5));
	ASSERT_TRUE(ackBuffer.put(6));
	ASSERT_FALSE(ackBuffer.put(4));
	ASSERT_TRUE(ackBuffer.put(10));
	ASSERT_FALSE(ackBuffer.put(7));
	ASSERT_FALSE(ackBuffer.put(8));
	ASSERT_TRUE(ackBuffer.put(5000));
	ASSERT_FALSE(ackBuffer.put(9));
}

TEST_F(AckBufferTest, handlesOverflowingSequences) {
	ASSERT_TRUE(ackBuffer.put(65534));
	ASSERT_TRUE(ackBuffer.put(65535));
	ASSERT_TRUE(ackBuffer.put(0));
	ASSERT_TRUE(ackBuffer.put(1));
}

TEST_F(AckBufferTest, fullBitfieldWhenAcksSequential) {
	for (int i = 0; i < 33; ++i) {
		ASSERT_TRUE(ackBuffer.put(i));
	}

	EXPECT_EQ(ackBuffer.last_sequence, 32);
	EXPECT_EQ(ackBuffer.ack_bitfield, 0xffffffff);
}
