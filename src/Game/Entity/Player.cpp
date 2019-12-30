#include "Player.h"

#include <iostream>
#include <stdint.h>


const Player::State Player::dummy_state = Player::State();

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
					std::cout << "new pos x (read): " << entity_state.pos_x << "\n";
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
	serialize(packet, dummy_state); // todo: get rid of dummy states, just pass an argument
}

void Player::serialize(OutPacket& packet, Entity& last_entity) {
	serialize(packet, dynamic_cast<const Player&>(last_entity).entity_state); // static_cast!
}

void Player::serialize(OutPacket& packet, const State& last_state) {
	ModFields modified_fields = ModFields();
	unsigned short modified_fields_i = packet.getBufferIndex();
	packet.write(modified_fields.raw);

	//std::cout << "last " << last_state.pos_x << " new " << entity_state.pos_x << "\n";

	// Write the changed data
	modified_fields.fields.pos_x = writeDeltaField(packet, entity_state.pos_x, last_state.pos_x);
	modified_fields.fields.pos_y = writeDeltaField(packet, entity_state.pos_y, last_state.pos_y);
	modified_fields.fields.score = writeDeltaField(packet, entity_state.score, last_state.score);

	unsigned short real_buffer_index = packet.getBufferIndex();
	packet.setBufferIndex(modified_fields_i);

	//if (modified_fields.raw != 0) { // Write the bitfield
		std::cout << "writing bitfield hehe\n";
		packet.write(modified_fields.raw);
		packet.setBufferIndex(real_buffer_index);
	//} else {
	//	std::cout << "last " << last_state.pos_x << " new " << entity_state.pos_x << "\n";
	//}
}
