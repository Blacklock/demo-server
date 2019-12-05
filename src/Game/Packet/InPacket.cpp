#include "InPacket.h"

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
	increase_buffer_index(1);
	return static_cast<UnreliableCmd>(buffer[buffer_index - 1]);
};

template<> ControlCmd InPacket::read<ControlCmd>() {
	increase_buffer_index(1);
	return static_cast<ControlCmd>(buffer[buffer_index - 1]);
};

template<> unsigned char InPacket::read<unsigned char>() {
	increase_buffer_index(1);
	return buffer[buffer_index - 1];
};

template<> unsigned short InPacket::read<unsigned short>() {
	increase_buffer_index(2);
	return (static_cast<unsigned short>(buffer[buffer_index - 2]) << 8) | buffer[buffer_index - 1];
};

template<> uint32_t InPacket::read<uint32_t>() {
	increase_buffer_index(4);
	return (uint32_t)buffer[buffer_index - 4] << 24 |
		(uint32_t)buffer[buffer_index - 3] << 16 |
		(uint32_t)buffer[buffer_index - 2] << 8 |
		(uint32_t)buffer[buffer_index - 1];
};

template<> int32_t InPacket::read<int32_t>() {
	return (int32_t)read<uint32_t>();
};

void InPacket::build(unsigned short buffer_size) {
	packet_length = buffer_size;

	packet_type = static_cast<PacketType>(read<unsigned char>());
	unsigned short packet_length_header = read<unsigned short>(); // todo: get rid of this header

	if (packet_length != packet_length_header) {
		throw std::exception("Packet length not equal to import size.");
	}

	if (packet_type == PacketType::Unreliable) {
		packet_sequence = read<unsigned short>();
		packet_ack = read<unsigned short>();
		ack_bitfield = read<uint32_t>();
	}

	/*std::cout << "PACKET RECEIVED\n";
	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";*/
}

void InPacket::increase_buffer_index(unsigned short amount) {
	buffer_index += amount;
	if (buffer_index > packet_length) {
		throw std::exception("Buffer index exceeds packet size.");
	}
}
