#pragma once

#include "gtest/gtest.h"
#include <stdint.h>
#include <stdexcept>

#include "../../../src/Utils/MessageQueue.cpp"


class MessageQueueTest : public ::testing::Test {
protected:
	MessageQueueTest() {
		queue = MessageQueue<unsigned char>();
	}

	MessageQueue<unsigned char> queue;
	unsigned char buffer[6] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	unsigned char out_buffer[64];
};

TEST_F(MessageQueueTest, emptyOnCreation) {
	ASSERT_EQ(queue.size(), 0);
}

TEST_F(MessageQueueTest, returnsCorrectSize) {
	queue.push(buffer, 3);
	EXPECT_EQ(queue.size(), 3);
	queue.push(buffer, 5);
	EXPECT_EQ(queue.size(), 8);
	queue.push(buffer, 1);
	EXPECT_EQ(queue.size(), 9);
}

TEST_F(MessageQueueTest, popsCorrectBytes) {
	queue.push(buffer, 2);
	queue.push(buffer, 6);
	queue.pop(out_buffer, 1);
	EXPECT_EQ(out_buffer[0], 'a');
	queue.pop(out_buffer, 4);
	EXPECT_EQ(out_buffer[0], 'b');
	EXPECT_EQ(out_buffer[1], 'a');
	EXPECT_EQ(out_buffer[2], 'b');
	EXPECT_EQ(out_buffer[3], 'c');
}

TEST_F(MessageQueueTest, handlesCopying) {
	queue.push(buffer, 6);
	queue.copy(out_buffer, 4);
	queue.copy(out_buffer, 3);
	EXPECT_EQ(out_buffer[0], 'a');
	EXPECT_EQ(out_buffer[1], 'b');
	EXPECT_EQ(out_buffer[2], 'c');
}

TEST_F(MessageQueueTest, handlesErasing) {
	queue.push(buffer, 6);
	queue.erase(3);
	queue.pop(out_buffer, 1);
	EXPECT_EQ(out_buffer[0], 'd');
}

TEST_F(MessageQueueTest, throwsOnOverflow) {
	queue.push(buffer, 6);
	queue.pop(out_buffer, 6);
	ASSERT_THROW(queue.pop(out_buffer, 1), std::out_of_range);
	queue.push(buffer, 6);
	ASSERT_THROW(queue.copy(out_buffer, 7), std::out_of_range);
	queue.erase(2);
	ASSERT_THROW(queue.erase(5), std::out_of_range);
	ASSERT_THROW(queue.copy(out_buffer, 5), std::out_of_range);
}
