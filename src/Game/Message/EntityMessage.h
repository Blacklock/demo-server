#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>


class EntityMessage {
/*public:
	std::unique_ptr<State> entity_state;
	static const std::unique_ptr<State> dummy_state;*/
public:
	//virtual void read(InPacket& packet) = 0;
	//virtual void serialize(OutPacket& packet, State& last_state) = 0;
protected:
	static bool writeDeltaField(OutPacket& packet, uint8_t new_field, uint8_t old_field);
	static bool writeDeltaField(OutPacket& packet, int32_t new_field, int32_t old_field, bool encode = true);
};
