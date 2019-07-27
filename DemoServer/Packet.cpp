#include "Packet.h"
#include "GameServer.h"

#include <iostream>
#include <bitset>


enum class ControlCommand : unsigned char {
	ConnRequest = 'R', // Client requesting connection [control command, protocol]
	ConnAccept = 'A', // Server accepting the client's connection request [control command]
	ConnDeny = 'D' // Server denying the client's connection request [control command]
};

Packet::Packet(unsigned char buffer[], int buffer_size) {
	packet_type = static_cast<PacketType>(buffer[0]); // Packet type
	packet_length = (static_cast<short>(buffer[2]) << 8) | buffer[1]; // Packet length
	//packet_length = (((short)buffer[2]) << 8) | buffer[1];

	switch (packet_type) {
		case PacketType::Reliable:
		case PacketType::Unreliable:
		default:
			throw std::invalid_argument("Unknown/unimplemented packet type.");

		case PacketType::Control:
			ControlCommand control_cmd = static_cast<ControlCommand>(buffer[3]);
			if (control_cmd == ControlCommand::ConnRequest) {
				unsigned char protocol_id = buffer[4];
				if (protocol_id == GameServer::GAME_PROTOCOL) {
					// 
					// ::: TODO :::
					// This packet is valid. GameServer will check the commands array of the packet,
					// and find one requesting a new (valid) connection. It will either accept or deny 
					// it. A new Packet with the response will be constructed and sent to the client.
					// 
				}
			}
			break;
	}

	/*char packet_data[256];
	memcpy(packet_data, &buffer[3], buffer_size - 3);*/

	// char* packet_data = new char[buffer_size];
	// strcpy_s(packet_data, buffer_size, (const char *)buffer + 3);

	std::cout << "Packet type: " << static_cast<std::underlying_type_t<PacketType>>(packet_type) << "\n";
	std::cout << "Packet length: " << packet_length << "\n";
	// std::cout << "Packet data: " << packet_data << "\n";

	// print buffer

	for (int i = 0; i < buffer_size; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";
}
