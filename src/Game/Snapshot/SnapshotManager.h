#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "../Client.h"
#include "Snapshot.h"


class SnapshotManager {
private:
	Snapshot master_snapshot = Snapshot(0); // id is 0?
	const PlayerState dummy_player = PlayerState();
public:
	SnapshotManager();
	void addPlayer(Client& client);
	void removePlayer(Client& client);
	void updatePlayerState(Client& client, InPacket& packet);
	void writeSnapshot(Client& client, OutPacket& packet);
	void writeDelta(OutPacket& packet, Snapshot* last_snapshot);

	static bool writeDeltaField(OutPacket& packet, uint8_t new_field, uint8_t old_field);
	static bool writeDeltaField(OutPacket& packet, int32_t new_field, int32_t old_field, bool encode = true);
};
