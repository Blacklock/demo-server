#include "GameServer.h"
#include "Packet.h"
#include "Socket.h"

#include <iostream>
#include <bitset>


Packet::Packet(unsigned char buffer_in[], PacketType type) {
	buffer = buffer_in;

	packet_type = type;
	packet_length = 256; // filler value

	// Set header (packet type, packet length, message id, ack bitfield...)

	append(static_cast<unsigned char>(packet_type));
	append(packet_length);
};

Packet::Packet(unsigned char buffer_in[], unsigned short import_size) {
	buffer = buffer_in;

	if (import_size > 0) {
		build(import_size);
	}
};

void Packet::setPacketLength() {
	// Set actual packet length
	packet_length = buffer_index;

	buffer_index = 1;
	append(packet_length);
	buffer_index = packet_length;


	for (int i = 0; i < packet_length; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";
}

void Packet::build(int buffer_size) {
	packet_type = static_cast<PacketType>(buffer[0]);
	packet_length = (static_cast<short>(buffer[1]) << 8) | buffer[2];

	switch (packet_type) {
		case PacketType::Control:
			packet_cmd.c_cmd = static_cast<ControlCmd>(buffer[3]);

			if (packet_cmd.c_cmd == ControlCmd::ConnRequest) {
				unsigned char protocol_id = buffer[4];
				if (protocol_id == GameServer::GAME_PROTOCOL) {
					break;
				}
			}
		case PacketType::Reliable:
		case PacketType::Unreliable:
			break;
	}

	// print buffer (temp)

	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";
}

void Packet::append(unsigned char value) {
	buffer[buffer_index] = value;
	buffer_index += 1;
}

void Packet::append(unsigned short value) {
	buffer[buffer_index] = (unsigned char)(value >> 8);
	buffer[buffer_index + 1] = (unsigned char)value;
	buffer_index += 2;
}
