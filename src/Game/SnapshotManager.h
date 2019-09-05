#pragma once

#include "Packet/InPacket.h"
#include "Client.h"
#include "Snapshot.h"


class SnapshotManager {
private:
	static Snapshot dummy_snapshot;
	Snapshot master_snapshot = Snapshot(nullptr);
public:
	SnapshotManager(Snapshot* dummy_snapshot);
	void updatePlayerState(InPacket& packet, Client& client);
};
