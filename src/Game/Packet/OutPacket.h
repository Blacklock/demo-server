#pragma once

#include "Packet.h"

#include <string>
#include <stdint.h>


class OutPacket : public Packet {
public:
	OutPacket(PacketType type, unsigned char buffer_in[], bool rel_switch = 0);
	void setHeaders(unsigned short sequence, unsigned short ack, uint32_t bitfield);

	void write(ControlCmd value);
	void write(UnreliableCmd value);
	void write(unsigned char value);
	void write(unsigned short value);
	void write(uint32_t value);
	void write(int32_t value);
	void write(std::string value);
};
