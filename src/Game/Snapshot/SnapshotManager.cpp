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

void SnapshotManager::writeSnapshot(OutPacket& packet, Client& client) {
	// Create a new delta-compressed snapshot
	Snapshot* new_snapshot = new Snapshot(client.server_sequence);

	// Get the latest snapshot acked by the client (for delta compression)
	Snapshot* last_snapshot = client.snapshots.get(client.last_snapshot);

	// If the latest snapshot isn't found in the buffer, use the master snapshot instead
	if (last_snapshot == nullptr) last_snapshot = &master_snapshot;

	// Add the new snapshot to the client's SnapshotBuffer
	client.snapshots.add(new_snapshot);

	// Iterate over all entities (currently just players)
	// PS: We are creating the iterator every single time ... we might not need to (same in game.cpp)?
	
	std::unordered_map<unsigned char, PlayerState*>::iterator entity;
	//for (std::unordered_map<unsigned char, PlayerState*>::iterator it; it != new_snapshot->player_states.end(); ++it) {
	for (entity = new_snapshot->player_states.begin(); entity != new_snapshot->player_states.end(); ++entity) {
		// Write the player ID (we might not actually need to send it every single time, look into it later)
		packet.write(entity->first); // const!

		// Find the given entity in last_snapshot
		PlayerState* last_entity = last_snapshot->player_states[entity->first];

		// Compare snapshot values
		// Whenever we add a new field, this has to be manually edited! Will look into a cleaner solution later

		ModifiedFields modified_fields = ModifiedFields();
		unsigned short modified_fields_bi = packet.getBufferIndex();
		packet.write(modified_fields.raw);

		if (entity->second->pos_x != last_entity->pos_x) {
			modified_fields.fields.pos_x = true;
			packet.write(entity->second->pos_x);
		}
		if (entity->second->pos_y != last_entity->pos_y) {
			modified_fields.fields.pos_y = true;
			packet.write(entity->second->pos_y);
		}
		if (entity->second->score != last_entity->score) {
			modified_fields.fields.score = true;
			packet.write(entity->second->score);
		}

		// Write the modified fields
		unsigned short real_buffer_index = packet.getBufferIndex();
		packet.setBufferIndex(modified_fields_bi);
		packet.write(modified_fields.raw);
		packet.setBufferIndex(real_buffer_index);
	}
}
