#pragma once

#include "../Client.h"
#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>


class ReliableMessage {
public:
	virtual void read(InPacket& packet) = 0; // Read entity data from a packet
	virtual void serialize(OutPacket& packet) = 0; // Write entity data to a packet
	virtual void on_ack(Client& client) = 0; // Reliable message has been confirmed
	virtual void on_fail(Client& client) = 0; // Reliable message has been skipped
};
