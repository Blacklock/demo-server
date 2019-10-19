#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "../Client.h"
#include "Snapshot.h"


class SnapshotManager {
private:
	Snapshot master_snapshot = Snapshot(0); // id is 0?
public:
	SnapshotManager();
	void updatePlayerState(InPacket& packet, Client& client);
	void writeSnapshot(OutPacket& packet, Client& client);
	void writeDelta(OutPacket& packet, Snapshot* new_snapshot, Snapshot* last_snapshot);

	bool writeDeltaField(OutPacket& packet, uint8_t new_field, uint8_t old_field);
	bool writeDeltaField(OutPacket& packet, int32_t new_field, int32_t old_field, bool encode = true);
};
