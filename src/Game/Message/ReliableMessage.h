#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"


class Client;

class ReliableMessage {
public:
	virtual void read(InPacket& packet) = 0; // Read entity data from a packet
	virtual void serialize(OutPacket& packet) = 0; // Write entity data to a packet
	virtual void onAck(Client& client) = 0; // Reliable message has been confirmed
	virtual void onFail(Client& client) = 0; // Reliable message has been skipped
};
