#pragma once

#include <stdint.h>


class AckBuffer {
public:
	unsigned short last_sequence;
	uint32_t ack_bitfield = 0;
public:
	AckBuffer(unsigned short start_sequence = 0);
	void put(unsigned short sequence);
	void reset(unsigned short start_sequence = 0);
};
