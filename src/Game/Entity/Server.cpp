#include "Server.h"

#include <iostream>
#include <stdint.h>


Server::Server() {
	entity_state = State();
}

Server::Server(State& state) {
	entity_state = state;
}

Server::Server(const Server& other) {
	entity_state = other.entity_state;
}

void Server::read(InPacket&) {
	throw std::exception("Entity isn't modifiable by clients.");
}

void Server::serialize(OutPacket& packet) {
	std::shared_lock lock(access);
	serialize(packet, entity_state, true);
}

void Server::serialize(OutPacket& packet, Entity& last_entity) {
	std::shared_lock lock(access);
	serialize(packet, static_cast<const Server&>(last_entity).entity_state);
}

void Server::serialize(OutPacket& packet, const State& last_state, bool dummy) {
	ModFields modified_fields = ModFields();
	unsigned short modified_fields_i = packet.getBufferIndex();
	packet.write(modified_fields.raw);

	// Write the changed data
	modified_fields.fields.status = writeDeltaField(packet, entity_state.status, last_state.status, dummy);

	unsigned short real_buffer_index = packet.getBufferIndex();
	packet.setBufferIndex(modified_fields_i);

	if (modified_fields.raw != 0) { // Write the bitfield
		packet.write(modified_fields.raw);
		packet.setBufferIndex(real_buffer_index);
	}
}
