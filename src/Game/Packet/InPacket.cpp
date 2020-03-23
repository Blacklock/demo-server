#include "InPacket.h"

#include <plog/Log.h>
#include <iostream>
#include <stdint.h>
//#include <bitset>


InPacket::InPacket(unsigned char buffer_in[], unsigned short import_size) {
	buffer = buffer_in;

	if (import_size > 0) {
		build(import_size);
	}
};

template<> UnreliableCmd InPacket::read<UnreliableCmd>() {
	increaseBufferIndex(1);
	return static_cast<UnreliableCmd>(buffer[buffer_index - 1]);
};

template<> ReliableCmd InPacket::read<ReliableCmd>() {
	increaseBufferIndex(1);
	return static_cast<ReliableCmd>(buffer[buffer_index - 1]);
};

template<> ControlCmd InPacket::read<ControlCmd>() {
	increaseBufferIndex(1);
	return static_cast<ControlCmd>(buffer[buffer_index - 1]);
};

template<> unsigned char InPacket::read<unsigned char>() {
	increaseBufferIndex(1);
	return buffer[buffer_index - 1];
};

template<> unsigned short InPacket::read<unsigned short>() {
	increaseBufferIndex(2);
	return (static_cast<unsigned short>(buffer[buffer_index - 2]) << 8) | buffer[buffer_index - 1];
};

template<> uint32_t InPacket::read<uint32_t>() {
	increaseBufferIndex(4);
	return (uint32_t)buffer[buffer_index - 4] << 24 |
		(uint32_t)buffer[buffer_index - 3] << 16 |
		(uint32_t)buffer[buffer_index - 2] << 8 |
		(uint32_t)buffer[buffer_index - 1];
};

template<> int32_t InPacket::read<int32_t>() {
	return (int32_t)read<uint32_t>();
};

std::string InPacket::readString(unsigned short size, bool encode) {
	if (encode) {
		LOGE << "String encoding not implemented!";
		throw std::exception("String encoding not implemented!");
	}

	increaseBufferIndex(size);
	return std::string(&buffer[buffer_index - size], &buffer[buffer_index]);
}

void InPacket::build(unsigned short buffer_size) {
	/*std::cout << "PACKET RECEIVED\n";
	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";*/

	packet_length = buffer_size;

	unsigned char header = read<unsigned char>();
	packet_type = static_cast<PacketType>(header >> 6);
	reliable_switch = (header >> 5) & 1;

	if (packet_type != PacketType::Control) {
		packet_sequence = read<unsigned short>();
		packet_ack = read<unsigned short>();
		ack_bitfield = read<uint32_t>();
	}
}

void InPacket::increaseBufferIndex(unsigned short amount) {
	buffer_index += amount;
	if (buffer_index > packet_length) {
		LOGE << "Buffer index exceeds packet size";
		throw std::out_of_range("Buffer index exceeds packet size.");
	}
}
