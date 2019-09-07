#pragma once

#include "Packet/InPacket.h"
#include "Packet/OutPacket.h"
#include "Client.h"
#include "Snapshot.h"


class SnapshotManager {
private:
	Snapshot dummy_snapshot;
	Snapshot master_snapshot = Snapshot(nullptr);

	unsigned char entities; // How many entities (players) are in the snapshot
public:
	SnapshotManager(Snapshot* dummy_snapshot);
	void updatePlayerState(InPacket& packet, Client& client);
	void sendSnapshot(OutPacket& packet, Client& client);
};
