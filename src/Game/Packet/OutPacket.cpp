#include "Packet.h"
#include "OutPacket.h"

#include <iostream>


OutPacket::OutPacket(PacketType type, unsigned char buffer_in[]) {
	buffer = buffer_in;
	packet_type = type;

	write(static_cast<unsigned char>(packet_type));

	// Reserve space for the header(s)
	buffer_index += packet_type == PacketType::Unreliable ? (2 + 2 + 2 + 4) : 2;
};

void OutPacket::setHeaders(unsigned short sequence, unsigned short ack, uint32_t ack_bitfield) {
	if (packet_type != PacketType::Unreliable) {
		throw std::logic_error("Headers can be set only on unreliable packets.");
	}

	unsigned short real_buffer_index = buffer_index;
	buffer_index = 3;

	write(sequence);
	write(ack);
	write(ack_bitfield);

	buffer_index = real_buffer_index;
}

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

void OutPacket::write(UnreliableCmd value) {
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

void OutPacket::write(uint32_t value) {
	buffer[buffer_index] = (unsigned char)(value >> 24);
	buffer[buffer_index + 1] = (unsigned char)(value >> 16);
	buffer[buffer_index + 2] = (unsigned char)(value >> 8);
	buffer[buffer_index + 3] = (unsigned char)value;
	buffer_index += 4;
}
