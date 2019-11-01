#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "Entity.h"


class PlayerEntity : public Entity {
public:
	struct State { // Player state fields & default dummy values
		int32_t pos_x = 0;
		int32_t pos_y = 0;
		uint8_t score = 0;
	};

	enum class Fields : unsigned char { // Shows what fields have changed
		PosX,
		PosY,
		Score,
		End
	};

	union ModFields { // Shows which fields have been modified
		struct {
			bool pos_x : 1;
			bool pos_y : 1;
			bool score : 1;
			bool empty : 5;
		} fields;
		unsigned char raw;
	};
public:
	PlayerEntity();
	void read(InPacket& packet);
	void serialize(OutPacket& packet);

	State entity_state; // inline later?

	static const State dummy_state;
};
