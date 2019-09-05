#include "SnapshotManager.h"

#include <unordered_map>


SnapshotManager::SnapshotManager(Snapshot* dummy_snapshot) :
	master_snapshot(nullptr),
	dummy_snapshot(dummy_snapshot) {}

SnapshotManager::updatePlayerState(InPacket& packet, Client& client) {
	// Find the player state struct or create a new one
	std::unordered_map<unsigned char, PlayerState*>::iterator player_state;
	player_state = master_snapshot.player_states.find(client.id);

	if (player_state == player_states.end()) { // Player state doesn't exist
		master_snapshot.player_states[client.id] = new PlayerState();
	}

	// Update the player state

	unsigned char field_flags = packet.read<unsigned char>();

	// Iterate over field flags

	for (int i = 0; i != (int)SnapshotFields::End; i++) {
		std::cout << i << "\n";

		if (field_flags & 1) { // Field has been changed
			switch ((SnapshotFields)i) {
				case SnapshotFields::PosX:
					player_state.pos_x = packet.read<int32_t>();
					break;
				case SnapshotFields::PosY:
					player_state.pos_y = packet.read<int32_t>();
					break;
				case SnapshotFields::Score:
					player_state.score = packet.read<unsigned char>();
					break;
				default:
					throw std::invalid_argument("Unknown snapshot field.");
			}
		}

		field_flags >>= 1;
	}
}
