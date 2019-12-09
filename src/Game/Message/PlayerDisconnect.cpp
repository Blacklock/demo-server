#include "PlayerDisconnect.h"


PlayerDisconnect::PlayerDisconnect(InPacket& packet) {
	read(packet);
};

PlayerDisconnect::PlayerDisconnect(Fields& data) : fields(data) {}

void PlayerDisconnect::read(InPacket& packet) {
	packet;
	throw std::exception("Client packets can't contain PlayerDisconnect messages.");
}

void PlayerDisconnect::serialize(OutPacket& packet) {
	packet.write(static_cast<unsigned char>(ReliableCmd::PlayerDisconnect));
	packet.write(fields.entity_id);
}
