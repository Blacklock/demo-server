#include "PlayerDisconnect.h"


PlayerDisconnect::PlayerDisconnect(InPacket& packet) {
	read(packet);
};

PlayerDisconnect::PlayerDisconnect(Fields& data) : message_fields(data) {}

void PlayerDisconnect::read(InPacket& packet) {
	packet;
}

void PlayerDisconnect::serialize(OutPacket& packet) {
	packet.write(static_cast<unsigned char>(ReliableCmd::PlayerDisconnect));
	packet.write(message_fields.entity_id);
}
