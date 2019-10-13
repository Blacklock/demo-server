#pragma once

#include "../Packet/InPacket.h"

#include <stdint.h>
#include <memory>
#include <unordered_map>


enum class SnapshotFields : unsigned char { // Shows what fields have changed
	PosX,
	PosY,
	Score,
	End
};

struct PlayerState { // Player state fields
	int32_t pos_x;
	int32_t pos_y;
	int32_t score;
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
	std::unordered_map<unsigned char, std::shared_ptr<PlayerState>> player_states;
public:
	Snapshot(unsigned short id);
};
