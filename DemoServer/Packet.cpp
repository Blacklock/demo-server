#include "Packet.h"
#include "GameServer.h"

#include <iostream>
#include <bitset>


Packet::Packet(unsigned char buffer[], int buffer_size) {
	packet_type = static_cast<PacketType>(buffer[0]); // Packet type
	packet_length = (static_cast<short>(buffer[1]) << 8) | buffer[2]; // Packet length
	//packet_length = (((short)buffer[2]) << 8) | buffer[1];

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
		default:
			throw std::invalid_argument("Invalid packet format provided.");
	}

	// print buffer (temp)

	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";
}
