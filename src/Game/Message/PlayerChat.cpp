#include "PlayerChat.h"


PlayerChat::PlayerChat(InPacket& packet) {
	read(packet);
};

PlayerChat::PlayerChat(Fields& data) : message_fields(data) {}

void PlayerChat::read(InPacket& packet) {
	message_fields.entity_id = packet.read<unsigned char>();
    message_fields.message_length = packet.read<unsigned char>();
    // message_fields.chat_message = packet read an std string with given length
}

void PlayerChat::serialize(OutPacket& packet) {
	packet.write(static_cast<unsigned char>(ReliableCmd::PlayerChat));
	packet.write(message_fields.entity_id);
    packet.write(message_fields.message_length);
    packet.write(message.fields.chat_message);
}
