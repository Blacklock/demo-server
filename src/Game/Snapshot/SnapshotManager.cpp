#include "SnapshotManager.h"
#include "../Entity/Player.h"
#include "../../Config.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <bitset>
#include <string>


SnapshotManager::SnapshotManager() {}

void SnapshotManager::updatePlayerState(Client& client, InPacket& packet) {
	// TODO: We should compare the client packet sequence to last_playerdata_received, if we aren't yet
	// Find the PlayerState struct or create a new one

	auto player_entity = master_snapshot.entities.find(client.id);

	if (player_entity == master_snapshot.entities.end()) { // Player state doesn't exist
		std::cout << "PlayerEntity wasn't found! Creating a new one.\n";
		master_snapshot.entities[client.id] = std::make_shared<Player>();
		player_entity = master_snapshot.entities.find(client.id);
	}

	//player_entity->second.read(packet); // Update the player state
	player_entity->second->read(packet); // Update the player state
}

void SnapshotManager::writeSnapshot(Client& client, OutPacket& packet) {
	if (config::DEBUG) std::cout << "[Snapshot Info]\n";
	if (config::DEBUG) std::cout << "[SID]\t" << static_cast<int>(client.server_sequence) << "\n";

	// Create a new delta-compressed snapshot
	std::shared_ptr<Snapshot> new_snapshot = std::make_shared<Snapshot>(client.server_sequence);

	// Get the latest snapshot acked by the client
	std::shared_ptr<Snapshot> last_snapshot = client.snapshots.get(client.last_snapshot);

	// Send a dummy snapshot if needed
	writeDelta(packet, last_snapshot == nullptr ? nullptr : last_snapshot.get(), client.id);

	// Deep copy master entity states into our new snapshot
	new_snapshot->entities = master_snapshot.entities;

	// Add the new snapshot to the client's SnapshotBuffer
	client.snapshots.add(new_snapshot);
}

// pass shared_ptr's instead of raw pointers? gotta figure out why we did this in the first place
void SnapshotManager::writeDelta(OutPacket& packet, Snapshot* last_snapshot, unsigned char client_id) {
	// Iterate over all entities in the master snapshot

	for (auto entity = master_snapshot.entities.begin(); entity != master_snapshot.entities.end(); ++entity) {
		if (entity->first == client_id) {
			continue; // Don't send the client information about itself
		}

		packet.write(entity->first); // Write the entity ID

		// Write the entity's delta fields

		if (last_snapshot == nullptr) { // Last snapshot doesn't exist anymore (too old), send all of the fields
			entity->second->serialize(packet); // If last_entity is nullptr, it's guaranteed to be sent again
		} else {
			// Find the given entity in last_snapshot
			auto last_entity_it = last_snapshot->entities.find(entity->first);

			if (last_entity_it != last_snapshot->entities.end()) {
				entity->second->serialize(packet, *last_entity_it->second);
			} else { // Entity not found
				entity->second->serialize(packet);
			}
		}

		if (config::DEBUG) std::cout << "[EID]\t" << static_cast<int>(entity->first) << "\n";
	}
}
