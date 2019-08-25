#include "Packet.h"
#include "OutPacket.h"


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

	/*std::cout << "SETPACKETLENGTH / SEND\n";
	for (int i = 0; i < packet_length; i++) {
		std::cout << std::bitset<8>(buffer[i]).to_string() << " ";
	}
	std::cout << "\n";*/
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
