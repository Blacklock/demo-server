#include "Snapshot.h"

#include <iostream>


Snapshot::Snapshot(Snapshot* source_snapshot) {
	modified_fields.raw = 0;

	if (!source_snapshot) { // nullptr, it's either a master or dummy snapshot
		// nothing yet
	}
}

// Reads a PlayerSnapshot and updates the master game state
/*void Snapshot::updatePlayerState(InPacket& packet, Client& client) {
	// Find the player state struct or create a new one
	std::unordered_map<unsigned char, PlayerState*>::iterator player_state;
	player_state = player_states.find(client.id);

	if (player_state == player_states.end()) { // Player state doesn't exist
		player_states[client.id] = new PlayerState();
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
}*/
