#include "Player.h"

#include <iostream>
#include <stdint.h>


Player::Player() {
	entity_state = State();
}


Player::Player(State& state) {
	entity_state = state;
}

void Player::read(InPacket& packet) {
	// Update the player state
	unsigned char field_flags = packet.read<unsigned char>();

	// Iterate over field flags
	for (int i = 0; i != (int)Fields::End; i++) {
		if (field_flags & 1) { // Field has been changed
			// In the future we will have to check the validity of the given data (anticheat)
			switch ((Fields)i) {
				case Fields::PosX:
					entity_state.pos_x = packet.read<int32_t>();
					break;
				case Fields::PosY:
					entity_state.pos_y = packet.read<int32_t>();
					break;
				case Fields::Score:
					entity_state.score = packet.read<uint8_t>();
					break;
				default:
					throw std::invalid_argument("Unknown PlayerEntity field.");
			}
		}

		field_flags >>= 1;
	}
}

void Player::serialize(OutPacket& packet) {
	serialize(packet, entity_state, true);
}

void Player::serialize(OutPacket& packet, Entity& last_entity) {
	serialize(packet, static_cast<const Player&>(last_entity).entity_state);
}

void Player::serialize(OutPacket& packet, const State& last_state, bool dummy) {
	ModFields modified_fields = ModFields();
	unsigned short modified_fields_i = packet.getBufferIndex();
	packet.write(modified_fields.raw);

	// Write the changed data
	modified_fields.fields.pos_x = writeDeltaField(packet, entity_state.pos_x, last_state.pos_x, dummy);
	modified_fields.fields.pos_y = writeDeltaField(packet, entity_state.pos_y, last_state.pos_y, dummy);
	modified_fields.fields.score = writeDeltaField(packet, entity_state.score, last_state.score, dummy);

	unsigned short real_buffer_index = packet.getBufferIndex();
	packet.setBufferIndex(modified_fields_i);

	if (modified_fields.raw != 0) { // Write the bitfield
		packet.write(modified_fields.raw);
		packet.setBufferIndex(real_buffer_index);
	}
}
