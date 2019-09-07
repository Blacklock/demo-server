#pragma once

#include "Packet/InPacket.h"
#include "Packet/OutPacket.h"
#include "Client.h"
#include "Snapshot.h"


class SnapshotManager {
private:
	Snapshot dummy_snapshot;
	Snapshot master_snapshot = Snapshot(nullptr);
public:
	SnapshotManager(Snapshot* dummy_snapshot);
	void updatePlayerState(InPacket& packet, Client& client);
	void writeSnapshot(OutPacket& packet, Client& client);
};
