#pragma once

#include <stdint.h>


class AckBuffer {
public:
	unsigned short last_sequence;
	uint32_t ack_bitfield = 0;
	bool empty = true;
public:
	AckBuffer();
	void put(unsigned short sequence);
	void reset();
};
