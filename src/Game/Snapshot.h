#pragma once

#include "Packet/InPacket.h"

#include <stdint.h>
#include <vector>


enum class SnapshotFields : unsigned char { // Shows what fields have changed
	PosX,
	PosY,
	Score,
	End
};

struct Snapshot {
	// Source snapshot to be compared with (delta compression)
	Snapshot* source_snapshot;

	// Player state fields
	struct PlayerState {
		int32_t pos_x;
		int32_t pos_y;
		unsigned char score;
	};

	// Empty vector of player states
	std::vector<PlayerState> player_states;

	// Shows which fields have been modified
	union {
		struct {
			bool pos_x : 1;
			bool pos_y : 1;
			bool score : 1;
			bool empty : 5;
		} fields;
		unsigned char raw;
	} modified_fields;

	Snapshot(Snapshot* source_snapshot);
	void read(InPacket packet);
};
