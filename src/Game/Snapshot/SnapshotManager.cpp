#include "SnapshotManager.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <bitset>
#include <string>


const bool DEBUG = false; // Debug mode (global variable; temp)

SnapshotManager::SnapshotManager() {}

void SnapshotManager::updatePlayerState(InPacket& packet, Client& client) {
	// TODO: We should compare the client packet sequence to last_playerdata_received, if we aren't yet
	// Find the PlayerState struct or create a new one

	std::unordered_map<unsigned char, PlayerState>::iterator player_state;
	player_state = master_snapshot.player_states.find(client.id);

	if (player_state == master_snapshot.player_states.end()) { // Player state doesn't exist
		master_snapshot.player_states[client.id] = PlayerState();
		player_state = master_snapshot.player_states.find(client.id);
	}

	// Update the player state

	unsigned char field_flags = packet.read<unsigned char>();

	// Iterate over field flags

	for (int i = 0; i != (int)PlayerFields::End; i++) {
		if (field_flags & 1) { // Field has been changed
			// In the future we will have to check the validity of the given data
			switch ((PlayerFields)i) {
				case PlayerFields::PosX:
					player_state->second.pos_x = packet.read<int32_t>();
					break;
				case PlayerFields::PosY:
					player_state->second.pos_y = packet.read<int32_t>();
					break;
				case PlayerFields::Score:
					player_state->second.score = packet.read<uint8_t>();
					break;
				default:
					throw std::invalid_argument("Unknown PlayerState field.");
			}
		}

		field_flags >>= 1;
	}
}

void SnapshotManager::writeSnapshot(OutPacket& packet, Client& client) {
	// Create a new delta-compressed snapshot
	std::shared_ptr<Snapshot> new_snapshot = std::make_shared<Snapshot>(client.server_sequence);

	// Get the latest snapshot acked by the client (is this safe? can the original pointer get deleted?)
	std::shared_ptr<Snapshot> last_snapshot = client.snapshots.get(client.last_snapshot);

	if (last_snapshot == nullptr) { // Latest snapshot wasn't found in the buffer
		writeDelta(packet, nullptr); // ...so send a dummy snapshot
	} else {
		writeDelta(packet, last_snapshot.get());
	}

	// Deep copy master player states into our new snapshot
	new_snapshot->player_states = master_snapshot.player_states;

	// Add the new snapshot to the client's SnapshotBuffer
	client.snapshots.add(new_snapshot);
}

// pass shared_ptr's instead of raw pointers? gotta figure out why we did this in the first place
void SnapshotManager::writeDelta(OutPacket& packet, Snapshot* last_snapshot) {
	if (DEBUG) std::cout << "[Snapshot Info]\n";

	// Iterate over all entities in the master snapshot (currently just players)
	std::unordered_map<unsigned char, PlayerState>::iterator entity;

	//for (entity = new_snapshot->player_states.begin(); entity != new_snapshot->player_states.end(); ++entity) {
	for (entity = master_snapshot.player_states.begin(); entity != master_snapshot.player_states.end(); ++entity) {
		// Write the player ID (if we add more entity types, this should become a combo of entity type + entity id)
		packet.write(entity->first); // const!

		PlayerState last_entity; // Find given entity in the last snapshot

		if (last_snapshot == nullptr) { // Last snapshot doesn't exist anymore (too old), send all of the fields
			last_entity = dummy_player; // If last_entity is nullptr, it's guaranteed to be sent again
		} else {
			// Find the given entity in last_snapshot
			std::unordered_map<unsigned char, PlayerState>::iterator last_entity_it;
			last_entity_it = last_snapshot->player_states.find(entity->first);

			if (last_entity_it != last_snapshot->player_states.end()) {
				last_entity = last_entity_it->second;
			} else { // Entity not found
				last_entity = dummy_player;
			}
		}

		// PlayerState last_entity = last_snapshot->player_states[entity->first];

		// Compare snapshot values
		// Whenever we add a new field, this has to be manually edited! Will look into a cleaner solution later

		ModifiedFields modified_fields = ModifiedFields();
		unsigned short modified_fields_bi = packet.getBufferIndex();
		packet.write(modified_fields.raw);

		if (DEBUG) std::cout << "[EID]\t" << static_cast<int>(entity->first) << "\n";

		if (DEBUG) std::cout << "\t[PosX]\t";
		modified_fields.fields.pos_x = writeDeltaField(packet, entity->second.pos_x, last_entity.pos_x);
		if (DEBUG) std::cout << "\t[PosY]\t";
		modified_fields.fields.pos_y = writeDeltaField(packet, entity->second.pos_y, last_entity.pos_y);
		if (DEBUG) std::cout << "\t[Score]\t";
		modified_fields.fields.score = writeDeltaField(packet, entity->second.score, last_entity.score);

		// Write the modified fields
		unsigned short real_buffer_index = packet.getBufferIndex();
		packet.setBufferIndex(modified_fields_bi);
		packet.write(modified_fields.raw);
		packet.setBufferIndex(real_buffer_index);
	}
}

bool SnapshotManager::writeDeltaField(OutPacket& packet, uint8_t new_field, uint8_t old_field) {
	if (new_field != old_field) {
		if (DEBUG) std::cout << static_cast<int>(new_field) << "\n";
		packet.write(new_field);
		return true;
	}
	if (DEBUG) std::cout << "Unchanged\n";
	return false;
}

bool SnapshotManager::writeDeltaField(OutPacket& packet, int32_t new_field, int32_t old_field, bool encode) {
	if (new_field != old_field) {
		if (DEBUG) std::cout << new_field << " (old field: " << old_field << ")\n";
		encode = false; // TEMP; we haven't implemented client-side leb128 decoding yet

		if (encode) { // (S)LEB128, vbyte encoding
			bool more = true;
			int sign = new_field >> 31; // 0 = unsigned; -1 = signed

			do {
				uint8_t byte = new_field & 0b01111111; // get the 7 least significant bits
				new_field >>= 7; // assumes arithmetic shift

				if (new_field == sign && ((byte ^ sign) & 0b01000000) == 0) {
					more = false;
					byte |= 0b10000000; // set the most significant bit to mark end
				}

				packet.write(byte);
			} while (more);
		} else {
			packet.write(new_field);
		}
		
		return true;
	}
	if (DEBUG) std::cout << "Unchanged\n";
	return false;
}
