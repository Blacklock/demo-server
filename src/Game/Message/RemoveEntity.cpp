#include "RemoveEntity.h"
#include "../Game.h"


RemoveEntity::RemoveEntity(InPacket& packet) {
	read(packet);
};

RemoveEntity::RemoveEntity(Fields& data) : fields(data) {}

void RemoveEntity::read(InPacket& packet) {
	packet;
	throw std::exception("Client packets can't contain RemoveEntity messages.");
}

void RemoveEntity::serialize(OutPacket& packet) {
	packet.write(static_cast<unsigned char>(ReliableCmd::RemoveEntity));
	packet.write(fields.entity_id);
}

void RemoveEntity::onAck(Client& client) {
	if (client.game->dead_entities.find(fields.entity_id) == client.game->dead_entities.end()) {
		throw std::exception("Dead entity ID not found.");
	}
	
	if (--client.game->dead_entities[fields.entity_id] <= 0) {
		// All clients have acked the entity's deletion, free the ID
		client.game->dead_entities.erase(fields.entity_id);
		client.game->id_slots.push(fields.entity_id);
	}
}

void RemoveEntity::onFail(Client& client) {
	onAck(client);
}
