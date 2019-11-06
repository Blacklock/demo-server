#include "PlayerEntity.h"
#include "../../Config.h"

#include <iostream>
#include <stdint.h>


//std::unique_ptr<State> PlayerEntity::dummy_state = std::make_unique<State>();
const PlayerEntity::State PlayerEntity::dummy_state = PlayerEntity::State();

PlayerEntity::PlayerEntity() {
	entity_state = State();
}

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

void PlayerEntity::serialize(OutPacket& packet, State& last_state) {
	// Compare snapshot values
	// Whenever we add a new field, this has to be manually edited! Will look into a cleaner solution later

	PlayerEntity::ModFields modified_fields = PlayerEntity::ModFields();
	unsigned short modified_fields_bi = packet.getBufferIndex();
	packet.write(modified_fields.raw);

	if (config::DEBUG) std::cout << "\t[PosX]\t";
	modified_fields.fields.pos_x = writeDeltaField(packet, entity_state.pos_x, last_state.pos_x);
	if (config::DEBUG) std::cout << "\t[PosY]\t";
	modified_fields.fields.pos_y = writeDeltaField(packet, entity_state.pos_y, last_state.pos_y);
	if (config::DEBUG) std::cout << "\t[Score]\t";
	modified_fields.fields.score = writeDeltaField(packet, entity_state.score, last_state.score);

	// Write the modified fields
	unsigned short real_buffer_index = packet.getBufferIndex();
	packet.setBufferIndex(modified_fields_bi);
	packet.write(modified_fields.raw);
	packet.setBufferIndex(real_buffer_index);
}
