#pragma once

#include "Packet/InPacket.h"
#include "Client.h"

#include <stdint.h>
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

// Shows which fields have been modified
union ModifiedFields {
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
	unsigned int id;
	// Map of player IDs/states
	std::unordered_map<unsigned char, PlayerState*> player_states;
private:
	// Source snapshot to be compared with (delta compression)
	Snapshot* source_snapshot;
public:
	Snapshot(unsigned short id, Snapshot* source_snapshot = nullptr);
};
