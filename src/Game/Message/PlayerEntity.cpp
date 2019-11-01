#include "PlayerEntity.h"

#include <iostream>


const PlayerEntity::State PlayerEntity::dummy_state = PlayerEntity::State();

PlayerEntity::PlayerEntity() : entity_state() {}

void PlayerEntity::read(InPacket& packet) {
	// Update the player state

	unsigned char field_flags = packet.read<unsigned char>();

	// Iterate over field flags

	for (int i = 0; i != (int)PlayerEntity::Fields::End; i++) {
		if (field_flags & 1) { // Field has been changed
			// In the future we will have to check the validity of the given data (anticheat)
			switch ((PlayerEntity::Fields)i) {
				case PlayerEntity::Fields::PosX:
					entity_state.pos_x = packet.read<int32_t>();
					break;
				case PlayerEntity::Fields::PosY:
					entity_state.pos_y = packet.read<int32_t>();
					break;
				case PlayerEntity::Fields::Score:
					entity_state.score = packet.read<uint8_t>();
					break;
				default:
					throw std::invalid_argument("Unknown PlayerState field.");
			}
		}

		field_flags >>= 1;
	}
}

void PlayerEntity::serialize(OutPacket& packet) {
	packet;
}
