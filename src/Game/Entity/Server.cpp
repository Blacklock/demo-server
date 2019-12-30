#include "Server.h"

#include <iostream>
#include <stdint.h>


const Server::State Server::dummy_state = Server::State();

Server::Server() {
	entity_state = State();
}

Server::Server(State& state) {
	entity_state = state;
}

void Server::read(InPacket&) {
	throw std::exception("Entity isn't modifiable by clients.");
}

void Server::serialize(OutPacket& packet) {
	serialize(packet, dummy_state);
}

void Server::serialize(OutPacket& packet, Entity& last_entity) {
	serialize(packet, static_cast<const Server&>(last_entity).entity_state);
}

void Server::serialize(OutPacket& packet, const State& last_state) {
	ModFields modified_fields = ModFields();
	unsigned short modified_fields_i = packet.getBufferIndex();
	packet.write(modified_fields.raw);

	// Write the changed data
	modified_fields.fields.status = writeDeltaField(packet, entity_state.status, last_state.status);

	unsigned short real_buffer_index = packet.getBufferIndex();
	packet.setBufferIndex(modified_fields_i);

	//if (modified_fields.raw != 0) { // Write the bitfield
		packet.write(modified_fields.raw);
		packet.setBufferIndex(real_buffer_index);
	//}
}
