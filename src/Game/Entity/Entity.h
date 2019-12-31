#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>


class Entity {
public:
	virtual void read(InPacket& packet) = 0;
	virtual void serialize(OutPacket& packet) = 0;
	virtual void serialize(OutPacket& packet, Entity& last_entity) = 0;
protected:
	static bool writeDeltaField(OutPacket& packet, uint8_t new_field, uint8_t old_field, bool dummy = false);
	static bool writeDeltaField(
		OutPacket& packet, int32_t new_field, int32_t old_field, bool dummy = false, bool encode = true);
};
