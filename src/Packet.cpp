#include "Packet.h"
#include "Socket.h"
#include "GameServer.h"

#include <iostream>
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
	/*return buffer[buffer_index - 4] << 24 |
		   buffer[buffer_index - 3] << 16 |
		   buffer[buffer_index - 2] << 8 |
		   buffer[buffer_index - 1];*/
};

void InPacket::build(int buffer_size) {
	packet_type = static_cast<PacketType>(buffer[0]);
	packet_length = (static_cast<unsigned short>(buffer[1]) << 8) | buffer[2]; // short => unsigned short

	/*std::cout << "BUILD\n";
	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";*/

	if (packet_length != buffer_size) {
		std::cout << "Packet length not equal to import size.\n";
	}
	
	buffer_index += 3;
}


OutPacket::OutPacket(PacketType type, unsigned char buffer_in[]) {
	buffer = buffer_in;
	packet_type = type;

	// Set header (packet type, packet length, message id, ack bitfield...)

	write(static_cast<unsigned char>(packet_type));
	buffer_index += 2; // Set the length later
};

void OutPacket::setPacketLength() {
	// Set actual packet length
	packet_length = buffer_index;

	buffer_index = 1;
	write(packet_length);
	buffer_index = packet_length;

	std::cout << "SETPACKETLENGTH\n";
	for (int i = 0; i < packet_length; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";
}

void OutPacket::write(ControlCmd value) {
	buffer[buffer_index] = static_cast<unsigned char>(value);
	buffer_index += 1;
}

void OutPacket::write(unsigned char value) {
	buffer[buffer_index] = value;
	buffer_index += 1;
}

void OutPacket::write(unsigned short value) {
	buffer[buffer_index] = (unsigned char)(value >> 8);
	buffer[buffer_index + 1] = (unsigned char)value;
	buffer_index += 2;
}
