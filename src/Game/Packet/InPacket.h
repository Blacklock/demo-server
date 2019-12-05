#pragma once

#include "Packet.h"

#include <stdint.h>


class InPacket : public Packet {
public:
	unsigned short packet_ack;
	uint32_t ack_bitfield;

	InPacket(unsigned char buffer_in[], int import_size);

	template<typename T> T read();
private:
	void build(int buffer_size);
	void increase_buffer_index(int amount);
};
