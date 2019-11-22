#include "PlayerDisconnect.h"


PlayerDisconnect::PlayerDisconnect(InPacket& packet) {
	read(packet);
};

PlayerDisconnect::PlayerDisconnect(Fields& data) : message_fields(data) {}

void PlayerDisconnect::read(InPacket& packet) {}

void PlayerDisconnect::serialize(OutPacket& packet) {
	
}
