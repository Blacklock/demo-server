#pragma once

#include "Packet.h"


class InPacket : public Packet {
public:
	InPacket(unsigned char buffer_in[], unsigned short import_size);

	template<typename T> T read();
private:
	void build(int buffer_size);
};