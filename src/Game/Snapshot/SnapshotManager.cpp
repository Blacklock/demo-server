#include "SnapshotManager.h"

#include <iostream>
#include <unordered_map>


SnapshotManager::SnapshotManager() {}

void SnapshotManager::updatePlayerState(InPacket& packet, Client& client) {
	// Find the player state struct or create a new one
	std::unordered_map<unsigned char, PlayerState*>::iterator player_state;
	player_state = master_snapshot.player_states.find(client.id);

	if (player_state == master_snapshot.player_states.end()) { // Player state doesn't exist
		master_snapshot.player_states[client.id] = new PlayerState();
		player_state = master_snapshot.player_states.find(client.id);
	}

	// Update the player state

	unsigned char field_flags = packet.read<unsigned char>();

	// Iterate over field flags

	for (int i = 0; i != (int)SnapshotFields::End; i++) {
		if (field_flags & 1) { // Field has been changed
			// In the future we will have to check the validity of the given data
			switch ((SnapshotFields)i) {
				case SnapshotFields::PosX:
					player_state->second->pos_x = packet.read<int32_t>();
					break;
				case SnapshotFields::PosY:
					player_state->second->pos_y = packet.read<int32_t>();
					break;
				case SnapshotFields::Score:
					player_state->second->score = packet.read<int32_t>();
					break;
				default:
					throw std::invalid_argument("Unknown snapshot field.");
			}
		}

		field_flags >>= 1;
	}

	std::cout << "[Master Gamestate]\nPosX\t" << master_snapshot.player_states[client.id]->pos_x <<
				 "\nPosY\t" << master_snapshot.player_states[client.id]->pos_y <<
				 "\nScore\t" << +master_snapshot.player_states[client.id]->score << "\n";
}

Snapshot* SnapshotManager::createSnapshot(Client& client) {
	// Create a new snapshot object
	Snapshot* new_snapshot = new Snapshot(client.server_sequence);

	// Get the latest snapshot acked by the client
	Snapshot* last_snapshot = client.snapshots.get(client.last_snapshot);

	/*if (last_snapshot->id != client.last_snapshot || last_snapshot == nullptr) {
		last_snapshot = &master_snapshot;
	}*/

	if (last_snapshot == nullptr) last_snapshot = &master_snapshot;

	// TODO: Delta compression
	// Not sure how exactly this will be done yet.
	// I would rather not store all non-changed fields in the PlayerState, or does it even matter that much?

	// Add the new snapshot to the client's SnapshotBuffer
	client.snapshots.add(new_snapshot);

	return new_snapshot;
}

void SnapshotManager::writeSnapshot(OutPacket& packet, Client& client) {
	// Create a new delta-compressed snapshot
	Snapshot* new_snapshot = createSnapshot(client);

	// Iterate over all entities (currently just players)
	// PS: We are creating the iterator every single time ... we might not need to (same in game.cpp)?
	// ==== we are just using master_snapshot for now and ignoring the newly created snapshot!!! ====
	for (std::unordered_map<unsigned char, PlayerState*>::iterator it; it != master_snapshot.player_states.end(); ++it) {
		// Write the player ID (we might not actually need to send it every single time, look into it later)
		packet.write(it->first); // const!
		
		// Write the snapshot fields (currently just write all the fields)

		// For now just create the ChangedFields struct manually
		// Later on it should become a member of Snapshot (and be set in the createSnapshot method)
		ModifiedFields modified_fields = ModifiedFields();
		modified_fields.fields.pos_x = true;
		modified_fields.fields.pos_y = true;
		modified_fields.fields.score = true;
		packet.write(modified_fields.raw);

		packet.write(it->second->pos_x);
		packet.write(it->second->pos_y);
		packet.write(it->second->score);
	}
}
