#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "ReliableMessage.h"

#include <memory>


class PlayerDisconnect : ReliableMessage {
public:
	struct Fields { // Player state fields & default dummy values
		uint8_t entity_id;
	};
private:
	Fields message_fields;
public:
	PlayerDisconnect(InPacket& packet);
	PlayerDisconnect(Fields& data);
	void read(InPacket& packet);
	void serialize(OutPacket& packet);
};
