#pragma once

#include "Packet/InPacket.h"

#include <stdint.h>


enum class SnapshotFields : unsigned char { // Shows what fields have changed
	PosX,
	PosY,
	Score,
	End
};

struct Snapshot {
	// Source snapshot to be compared with (delta compression)
	Snapshot* source_snapshot;

	// Fields
	int32_t pos_x;
	int32_t pos_y;
	unsigned char score;

	// Shows which fields have been modified
	union {
		struct {
			bool pos_x : 1;
			bool pos_y : 1;
			bool score : 1;
		};
		unsigned char raw;
	} snapshot_bitfield;

	Snapshot(Snapshot* source_snapshot);
	void read(InPacket packet);
};
