#pragma once

#include <stdint.h>


class AckBuffer {
public:
	unsigned short last_sequence = 0;
	uint32_t ack_bitfield = 0;
	bool empty = true;
public:
	AckBuffer();
	bool put(unsigned short sequence);
	void reset();
};
