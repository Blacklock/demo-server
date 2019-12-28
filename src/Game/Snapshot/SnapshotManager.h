#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "../Client.h"
#include "Snapshot.h"


class SnapshotManager {
public:
	Snapshot master_snapshot = Snapshot(0); // id is 0?
public:
	SnapshotManager();
	void updatePlayerState(Client& client, InPacket& packet);

	void writeSnapshot(Client& client, OutPacket& packet);
	void writeDelta(OutPacket& packet, Snapshot* last_snapshot, unsigned char client_id);
};
