#pragma once

#include "Packet.h"


class OutPacket : public Packet {
public:
	OutPacket(PacketType type, unsigned char buffer_in[]);

	void setPacketLength();

	void write(ControlCmd value);
	void write(unsigned char value);
	void write(unsigned short value);
};
