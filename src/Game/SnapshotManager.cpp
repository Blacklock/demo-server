#include "SnapshotManager.h"

#include <iostream>
#include <unordered_map>


SnapshotManager::SnapshotManager(Snapshot* dummy_snapshot) :
	dummy_snapshot(dummy_snapshot) {}

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
			//std::cout << i << " changed\n";
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
