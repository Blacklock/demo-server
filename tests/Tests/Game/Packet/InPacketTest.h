#pragma once

#include "gtest/gtest.h"
#include <stdint.h>

#include "../../../../src/Game/Packet/InPacket.cpp"


TEST(InPacketTest, handlesControlPackets) {
	unsigned char buffer[8] = { // ConnRequest packet
		static_cast<unsigned char>(PacketType::Control), // packet type
		0x00, 0x08, // packet length
		static_cast<unsigned char>(ControlCmd::ConnRequest), // command
		0x00, 0x01, 0x87, 0x04 // protocol id
	};
	InPacket packet = InPacket(buffer, 8);

	EXPECT_EQ(packet.packet_type, PacketType::Control);
	EXPECT_EQ(packet.packet_length, 8);
}

TEST(InPacketTest, handlesUnreliablePackets) {
	unsigned char buffer[17] = { // Empty PlayerData packet
		static_cast<unsigned char>(PacketType::Unreliable), // packet type
		0x00, 0x11, // packet length
		0x00, 0x2a, // sequence
		0x00, 0x0d, // ack
		0x00, 0x00, 0x01, 0x01, // ack bitfield
		static_cast<unsigned char>(UnreliableCmd::PlayerData), // command
		0b10000000, // fields bitfield (pos_x changed)
		0xff, 0xff, 0x00, 0x01 // pos_x field
	};
	InPacket packet = InPacket(buffer, 17);

	EXPECT_EQ(packet.packet_type, PacketType::Unreliable);
	EXPECT_EQ(packet.packet_length, 17);
	EXPECT_EQ(packet.packet_sequence, 42);
	EXPECT_EQ(packet.packet_ack, 13);
	EXPECT_EQ(packet.ack_bitfield, 257);
	EXPECT_EQ(packet.read<UnreliableCmd>(), UnreliableCmd::PlayerData);
	EXPECT_EQ(packet.read<unsigned char>(), 0b10000000);
	EXPECT_EQ(packet.read<int32_t>(), -65535);
}