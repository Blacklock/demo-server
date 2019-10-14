#include "SnapshotManager.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <bitset>


SnapshotManager::SnapshotManager() {}

void SnapshotManager::updatePlayerState(InPacket& packet, Client& client) {
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

	for (int i = 0; i != (int)SnapshotFields::End; i++) {
		if (field_flags & 1) { // Field has been changed
			// In the future we will have to check the validity of the given data
			switch ((SnapshotFields)i) {
				case SnapshotFields::PosX:
					player_state->second.pos_x = packet.read<int32_t>();
					break;
				case SnapshotFields::PosY:
					player_state->second.pos_y = packet.read<int32_t>();
					break;
				case SnapshotFields::Score:
					player_state->second.score = packet.read<uint8_t>();
					break;
				default:
					throw std::invalid_argument("Unknown snapshot field.");
			}
		}

		field_flags >>= 1;
	}

	/*std::cout << "[Master Gamestate]\nPosX\t" << master_snapshot.player_states[client.id].pos_x <<
				 "\nPosY\t" << master_snapshot.player_states[client.id].pos_y <<
				 "\nScore\t" << master_snapshot.player_states[client.id].score << "\n";*/
}

void SnapshotManager::writeSnapshot(OutPacket& packet, Client& client) {
	// All this smart pointer code looks ugly...
	// Create a new delta-compressed snapshot
	std::shared_ptr<Snapshot> new_snapshot = std::make_shared<Snapshot>(client.server_sequence);

	// Get the latest snapshot acked by the client (is this safe? can the original pointer get deleted?)
	std::shared_ptr<Snapshot> last_snapshot = client.snapshots.get(client.last_snapshot);

	// If the latest snapshot isn't found in the buffer, use the master snapshot instead
	if (last_snapshot == nullptr) {
		writeDelta(packet, new_snapshot.get(), last_snapshot.get());
	} else {
		writeDelta(packet, new_snapshot.get(), &master_snapshot);
	}

	// Add the new snapshot to the client's SnapshotBuffer
	client.snapshots.add(new_snapshot);
}

void SnapshotManager::writeDelta(OutPacket& packet, Snapshot* new_snapshot, Snapshot* last_snapshot) {
	// Iterate over all entities (currently just players)
	std::unordered_map<unsigned char, PlayerState>::iterator entity;

	/*writeDeltaField(packet, (int32_t)500, 0);
	writeDeltaField(packet, (int32_t)-500, 0);
	writeDeltaField(packet, (int32_t)-8191, 0);
	writeDeltaField(packet, (int32_t)-8192, 0);
	writeDeltaField(packet, (int32_t)8191, 0);
	writeDeltaField(packet, (int32_t)8192, 0);
	writeDeltaField(packet, (int32_t)-1028576, 0);
	writeDeltaField(packet, (int32_t)1050000, 0);*/

	for (entity = new_snapshot->player_states.begin(); entity != new_snapshot->player_states.end(); ++entity) {
		// Write the player ID (we might not actually need to send it every single time, look into it later)
		packet.write(entity->first); // const!

		// Find the given entity in last_snapshot
		PlayerState last_entity = last_snapshot->player_states[entity->first];

		// Compare snapshot values
		// Whenever we add a new field, this has to be manually edited! Will look into a cleaner solution later

		ModifiedFields modified_fields = ModifiedFields();
		unsigned short modified_fields_bi = packet.getBufferIndex();
		packet.write(modified_fields.raw);

		modified_fields.fields.pos_x = writeDeltaField(packet, entity->second.pos_x, last_entity.pos_x);
		modified_fields.fields.pos_y = writeDeltaField(packet, entity->second.pos_y, last_entity.pos_y);
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
		packet.write(new_field);
		return true;
	}
	return false;
}

bool SnapshotManager::writeDeltaField(OutPacket& packet, int32_t new_field, int32_t old_field, bool encode) {
	std::cout << new_field << "\n";
	if (new_field != old_field) {
		if (encode) { // (S)LEB128, vbyte encoding
			bool more = true;
			int sign = new_field >> 31; // 0 = unsigned; -1 = signed

			do {
				std::cout << "c";
				uint8_t byte = new_field & 0b01111111; // get the 7 least significant bits
				new_field >>= 7; // assumes arithmetic shift

				// more =  !((value == 0 && sign bit of byte is clear) || (value == -1 && sign bit of byte is set))
				if (new_field == sign && ((byte ^ sign) & 0b01000000) == 0) {
					more = false;
					byte |= 0b10000000; // set the most significant bit to mark end
				}
			} while (more);
			std::cout << "/\n";
		}
		
		packet.write(new_field);
		return true;
	}
	return false;
}

int32_t SnapshotManager::vbyteEncode(int32_t num) { // LEB128
	bool more = true;
	int sign = num >> 31; // 0 = unsigned; -1 = signed

	do {
		uint8_t byte = num & 0b01111111; // get the 7 least significant bits
		num >>= 7; // assumes arithmetic shift

		// more =  !((value == 0 && sign bit of byte is clear) || (value == -1 && sign bit of byte is set))
		// more = num != sign || ((byte ^ sign) & 0b1000000) != 0;
		if (num == sign && ((byte ^ sign) & 0b01000000) == 0) {
			more = false;
			byte |= 0b10000000; // set the most significant bit to mark end
		}
	} while (more);


	return 0; // temp
}
