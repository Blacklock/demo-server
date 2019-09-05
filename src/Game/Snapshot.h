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
	unsigned char score;
};

// Shows which player state fields have been modified
union ModifiedFields {
	struct {
		bool pos_x : 1;
		bool pos_y : 1;
		bool score : 1;
		bool empty : 5;
	} fields;
	unsigned char raw;
};

struct Snapshot {
	// Snapshot packet sequence
	unsigned short sequence;

	// Source snapshot to be compared with (delta compression)
	Snapshot* source_snapshot;

	// Map of player IDs and their states
	std::unordered_map<unsigned char, PlayerState> player_states;

	Snapshot(Snapshot* source_snapshot);
	void read(InPacket packet);
};
