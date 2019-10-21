#pragma once

#include "gtest/gtest.h"
#include <stdint.h>

#include "../../../../src/Game/Packet/OutPacket.cpp"


class OutPacketTest : public ::testing::Test {
protected:
	unsigned char buffer[512];
};

TEST_F(OutPacketTest, writesPacketLength) {
	OutPacket packet = OutPacket(PacketType::Control, buffer);
	packet.setPacketLength();
	ASSERT_EQ(packet.buffer[2], 0x03);
}

TEST_F(OutPacketTest, writesControlHeaders) {
	OutPacket packet = OutPacket(PacketType::Control, buffer);
	ASSERT_EQ(packet.getBufferIndex(), 3);
	ASSERT_EQ(packet.buffer[0], static_cast<unsigned char>(PacketType::Control));
}

TEST_F(OutPacketTest, writesUnreliableHeaders) {
	OutPacket packet = OutPacket(PacketType::Unreliable, buffer);
	ASSERT_EQ(packet.buffer[0], static_cast<unsigned char>(PacketType::Unreliable));
	ASSERT_EQ(packet.getBufferIndex(), 11);
	packet.setHeaders(5, 3, 0x01);
	EXPECT_EQ(packet.buffer[4], 0x05);
	EXPECT_EQ(packet.buffer[6], 0x03);
	EXPECT_EQ(packet.buffer[10], 0x01);
}

TEST_F(OutPacketTest, writesBytes) {
	OutPacket packet = OutPacket(PacketType::Control, buffer);
	packet.write(static_cast<unsigned char>(42));
	EXPECT_EQ(packet.buffer[3], 42);
	packet.write(ControlCmd::ConnAccept);
	EXPECT_EQ(packet.buffer[4], static_cast<unsigned char>(ControlCmd::ConnAccept));
	packet.write(UnreliableCmd::Snapshot);
	EXPECT_EQ(packet.buffer[5], static_cast<unsigned char>(UnreliableCmd::Snapshot));
}

TEST_F(OutPacketTest, writesUnsignedShorts) {
	OutPacket packet = OutPacket(PacketType::Control, buffer);
	packet.write(static_cast<unsigned short>(258));
	ASSERT_EQ(packet.buffer[3], 0x01);
	ASSERT_EQ(packet.buffer[4], 0x02);
}

TEST_F(OutPacketTest, writesUnsignedInts) {
	OutPacket packet = OutPacket(PacketType::Control, buffer);
	packet.write(static_cast<uint32_t>(65538));
	ASSERT_EQ(packet.buffer[4], 0x01);
	ASSERT_EQ(packet.buffer[6], 0x02);
}

TEST_F(OutPacketTest, writesSignedInts) {
	OutPacket packet = OutPacket(PacketType::Control, buffer);
	packet.write(static_cast<int32_t>(-65532));
	ASSERT_EQ(packet.buffer[5], 0x00);
	ASSERT_EQ(packet.buffer[6], 0x04);
}
