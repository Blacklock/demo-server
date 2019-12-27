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
	void addPlayer(Client& client);
	void removeEntity(unsigned char id);
	void updatePlayerState(Client& client, InPacket& packet);

	void writeSnapshot(Client& client, OutPacket& packet);
	void writeDelta(OutPacket& packet, Snapshot* last_snapshot, unsigned char client_id);
};
