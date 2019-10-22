#pragma once

#include "gtest/gtest.h"
#include <stdint.h>

#include "../../src/Socket.cpp"


class SocketTest : public ::testing::Test {
protected:
	unsigned long LOCAL_ADDRESS = 2130706433;
	const unsigned short LOCAL_PORT = 13370;
	const unsigned int MAX_PACKET_SIZE = 512;

	Socket socket = Socket(MAX_PACKET_SIZE);
	unsigned char buffer[512];
};

TEST_F(SocketTest, canSendPackets) {
	socket.create(LOCAL_PORT);
	short destPort = 13371;

	// ConnAccept packet
	buffer[0] = 0x02;
	buffer[1] = 'A';

	socket.sendPacket(buffer, 2, LOCAL_ADDRESS, destPort);
}

TEST_F(SocketTest, canReceivePackets) {
	socket.create(LOCAL_PORT);
	// Send buffer
	buffer[0] = 0x02;
	buffer[1] = 'R';
	buffer[2] = 0x00;
	socket.sendPacket(buffer, 3, LOCAL_ADDRESS, LOCAL_PORT);

	// Receive packet that we just sent (reuse sendBuffer as recvBuffer)
	InPacketInfo pi = socket.receivePacket(buffer);

	EXPECT_EQ(pi.buffer_size, 3);
	EXPECT_EQ(pi.sender_address, LOCAL_ADDRESS);
	EXPECT_EQ(pi.sender_port, LOCAL_PORT);
	EXPECT_EQ(buffer[0], 0x02);
	EXPECT_EQ(buffer[1],'R');
	EXPECT_EQ(buffer[2], 0x00);
}
