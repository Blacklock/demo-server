#pragma once

#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"
#include "ReliableMessage.h"

#include <memory>
#include <string>


class PlayerChat : public ReliableMessage {
public:
	struct Fields { // Player state fields & default dummy values
		uint8_t entity_id;
        uint8_t message_length;
        std::string chat_message;
	};
private:
	Fields message_fields;
public:
	PlayerChat(InPacket& packet);
	PlayerChat(Fields& data);
	void read(InPacket& packet);
	void serialize(OutPacket& packet);
};
