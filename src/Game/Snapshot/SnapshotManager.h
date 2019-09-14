#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "../Client.h"
#include "Snapshot.h"


class SnapshotManager {
private:
	Snapshot master_snapshot = Snapshot(0, nullptr); // id is 0?
public:
	SnapshotManager();
	void updatePlayerState(InPacket& packet, Client& client);
	Snapshot* createSnapshot(Snapshot& source_snapshot, Client& client);
	void writeSnapshot(OutPacket& packet, Client& client);
};
