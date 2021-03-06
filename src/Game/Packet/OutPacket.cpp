#include "Packet.h"
#include "OutPacket.h"

#include <plog/Log.h>
#include <iostream>
//#include <bitset>


OutPacket::OutPacket(PacketType type, unsigned char buffer_in[], bool rel_switch) {
	buffer = buffer_in;
	packet_type = type;

	unsigned char header = (static_cast<unsigned char>(packet_type) << 6) | (rel_switch << 5);
	write(header);

	// Reserve space for the headers
	if (packet_type != PacketType::Control) buffer_index += (2 + 2 + 4);
};

void OutPacket::setHeaders(unsigned short sequence, unsigned short ack, uint32_t bitfield) {
	if (packet_type == PacketType::Control) {
		LOGE << "Headers cannot be set on control packets";
		throw std::logic_error("Headers cannot be set on control packets.");
	}

	packet_sequence = sequence;

	unsigned short real_buffer_index = buffer_index;
	buffer_index = 1;

	write(sequence);
	write(ack);
	write(bitfield);

	buffer_index = real_buffer_index;

	/*std::cout << "PACKET SEND\n";
	for (int i = 0; i < buffer_index; i++) {
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

void OutPacket::write(int32_t value) {
	write((uint32_t)value);
}

void OutPacket::write(std::string value) {
	size_t size = value.size();
	memcpy(&buffer[buffer_index], value.c_str(), size);
	buffer_index += static_cast<unsigned short>(size);
}
