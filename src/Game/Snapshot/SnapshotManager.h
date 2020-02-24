#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "../Client.h"
#include "Snapshot.h"

#include <memory>


class SnapshotManager {
public:
	Snapshot master_snapshot = Snapshot(0); // id is 0?
public:
	SnapshotManager();

	template<class T> std::shared_ptr<T> getEntity(unsigned char id) {
		std::shared_ptr<T> entity = std::dynamic_pointer_cast<T>(master_snapshot.entities[id]);
		std::shared_ptr<T> new_entity = std::make_shared<T>(*entity);
		master_snapshot.entities[id] = new_entity;
		return new_entity;
	}

	bool writeSnapshot(Client& client, OutPacket& packet);
	bool writeDelta(OutPacket& packet, Snapshot* last_snapshot, unsigned char entity_id);
};
