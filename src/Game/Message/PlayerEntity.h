#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "Entity.h"


class PlayerEntity : public Entity {
public:
	enum class PlayerFields : unsigned char { // Shows what fields have changed
		PosX,
		PosY,
		Score,
		End
	};

	struct PlayerState { // Player state fields & default dummy values
		int32_t pos_x = 0;
		int32_t pos_y = 0;
		uint8_t score = 0;
	};

	union ModifiedFields { // Shows which fields have been modified
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

	PlayerState player_state; // inline later?
};
