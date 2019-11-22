#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>


class ReliableMessage {
public:
	virtual void read(InPacket& packet) = 0;
	virtual void serialize(OutPacket& packet) = 0;
};
