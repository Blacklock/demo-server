#include "RemoveEntity.h"


RemoveEntity::RemoveEntity(InPacket& packet) {
	read(packet);
};

RemoveEntity::RemoveEntity(Fields& data) : fields(data) {}

void RemoveEntity::read(InPacket& packet) {
	packet;
	throw std::exception("Client packets can't contain PlayerDisconnect messages.");
}

void RemoveEntity::serialize(OutPacket& packet) {
	packet.write(static_cast<unsigned char>(ReliableCmd::RemoveEntity));
	packet.write(fields.entity_id);
}
