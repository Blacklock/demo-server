#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "../Client.h"
#include "Snapshot.h"

#include <shared_mutex>
#include <unordered_map>
#include <memory>


class SnapshotManager {
public:
	std::unordered_map<unsigned char, std::shared_ptr<Entity>> master_snapshot;
private:
	//std::shared_ptr<Snapshot> cached_snapshot;
	std::shared_ptr<std::unordered_map<unsigned char, std::shared_ptr<Entity>>> cached_entities;
	std::shared_mutex cache_access;
public:
	SnapshotManager();

	template<class T> std::shared_ptr<T> getEntity(unsigned char id) {
		std::shared_ptr<T> entity = std::dynamic_pointer_cast<T>(master_snapshot[id]);
		std::shared_ptr<T> new_entity = std::make_shared<T>(*entity);
		master_snapshot[id] = new_entity;
		return new_entity;
	}

	void cacheEntities();
	bool writeSnapshot(Client& client, OutPacket& packet);
	bool writeDelta(OutPacket& packet, Snapshot* last_snapshot, unsigned char entity_id);
};
