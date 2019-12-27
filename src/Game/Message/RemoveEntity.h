#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "ReliableMessage.h"

#include <memory>
#include <stdint.h>


class RemoveEntity : public ReliableMessage {
public:
	struct Fields {
		uint8_t entity_id;
	} fields;
public:
	RemoveEntity(InPacket& packet);
	RemoveEntity(Fields& data);
	void read(InPacket& packet);
	void serialize(OutPacket& packet);
};
