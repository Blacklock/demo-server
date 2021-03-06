#pragma once

#include "Packet.h"

#include <string>
#include <stdint.h>


class InPacket : public Packet {
public:
	unsigned short packet_length;
	unsigned short packet_ack;
	uint32_t ack_bitfield;

	InPacket(unsigned char buffer_in[], unsigned short import_size);

	template<typename T> T read();
	std::string readString(unsigned short size, bool encode = false);
private:
	void build(unsigned short buffer_size);
	void increaseBufferIndex(unsigned short amount);
};
