#pragma once

#include "Packet.h"

#include <stdint.h>


class InPacket : public Packet {
public:
	unsigned short packet_ack;
	uint32_t ack_bitfield;

	InPacket(unsigned char buffer_in[], unsigned short import_size);

	template<typename T> T read();
private:
	void build(unsigned short buffer_size);
	void increase_buffer_index(unsigned short amount);
};
