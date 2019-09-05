#pragma once

#include "Packet/InPacket.h"
#include "Client.h"

#include <stdint.h>
#include <vector>
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

class Snapshot {
public:
	// Map of player IDs/states
	std::unordered_map<unsigned char, PlayerState*> player_states;
private:
	// Source snapshot to be compared with (delta compression)
	Snapshot* source_snapshot;

	// Empty vector of player states
	//std::vector<PlayerState> player_states;

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
public:
	Snapshot(Snapshot* source_snapshot);
	void updateGameState(InPacket& packet, Client& client);
	//void read(InPacket packet);
};
