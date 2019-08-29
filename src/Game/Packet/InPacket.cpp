#include "InPacket.h"

#include <iostream>
#include <stdint.h>
#include <bitset>


InPacket::InPacket(unsigned char buffer_in[], unsigned short import_size) {
	buffer = buffer_in;

	if (import_size > 0) {
		build(import_size);
	}
};

template<> UnreliableCmd InPacket::read<UnreliableCmd>() {
	buffer_index += 1;
	return static_cast<UnreliableCmd>(buffer[buffer_index - 1]);
};

template<> ControlCmd InPacket::read<ControlCmd>() {
	buffer_index += 1;
	return static_cast<ControlCmd>(buffer[buffer_index - 1]);
};

template<> unsigned char InPacket::read<unsigned char>() {
	buffer_index += 1;
	return buffer[buffer_index - 1];
};

template<> unsigned short InPacket::read<unsigned short>() {
	buffer_index += 2;
	return (static_cast<unsigned short>(buffer[buffer_index - 2]) << 8) | buffer[buffer_index - 1];
};

template<> int32_t InPacket::read<int32_t>() {
	buffer_index += 4;
	return (int32_t)buffer[buffer_index - 4] << 24 |
		(int32_t)buffer[buffer_index - 3] << 16 |
		(int32_t)buffer[buffer_index - 2] << 8 |
		(int32_t)buffer[buffer_index - 1];
};

template<> uint32_t InPacket::read<uint32_t>() {
	buffer_index += 4;
	return (uint32_t)buffer[buffer_index - 4] << 24 |
		(uint32_t)buffer[buffer_index - 3] << 16 |
		(uint32_t)buffer[buffer_index - 2] << 8 |
		(uint32_t)buffer[buffer_index - 1];
};

void InPacket::build(int buffer_size) {
	packet_type = static_cast<PacketType>(buffer[0]);
	packet_length = (static_cast<unsigned short>(buffer[1]) << 8) | buffer[2]; // short => unsigned short

	buffer_index += 3;

	if (packet_type == PacketType::Unreliable) {
		packet_sequence = (static_cast<unsigned short>(buffer[3]) << 8) | buffer[4];
		// std::cout << "Sequence: " << packet_sequence << "\n";
		buffer_index += 2;
	}

	std::cout << "BUILD\n";
	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";

	if (packet_length != buffer_size) {
		std::cout << "Packet length not equal to import size.\n";
	}
}
