#pragma once

#include "gtest/gtest.h"
#include <stdint.h>

#include "../../../src/Utils/MessageQueue.cpp"


class MessageQueueTest : public ::testing::Test {
protected:
	MessageQueueTest() {
		queue = MessageQueue<unsigned char>();
	}

	MessageQueue<unsigned char> queue;
};

TEST_F(MessageQueueTest, queueEmptyOnCreation) {
	ASSERT_EQ(queue.size(), 0);
}

TEST_F(MessageQueueTest, returnsCorrectSize) {
	unsigned char buffer[512] = {'a', 'b', 'c', 'd', 'e'};
	queue.push(buffer, 3);
	ASSERT_EQ(queue.size(), 3);
	queue.push(buffer, 5);
	ASSERT_EQ(queue.size(), 8);
	queue.push(buffer, 1);
	ASSERT_EQ(queue.size(), 9);
}
