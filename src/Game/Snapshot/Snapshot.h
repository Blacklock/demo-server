#pragma once

#include "../Packet/InPacket.h"

#include <stdint.h>
#include <memory>
#include <unordered_map>


enum class PlayerFields : unsigned char { // Shows what fields have changed
	PosX,
	PosY,
	Score,
	End
};

struct PlayerState { // Player state fields
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

class Snapshot {
public:
	// Snapshot packet ID
	const unsigned short id;
	// Map of player IDs/states
	// std::unordered_map<unsigned char, std::shared_ptr<PlayerState>> player_states;
	std::unordered_map<unsigned char, PlayerState> player_states;
public:
	Snapshot(unsigned short id);
};
