#pragma once

#include "ReliableMessage.h"
#include "../Client.h"
#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>
#include <string>


class PlayerChat : public ReliableMessage {
public:
	struct Fields {
		uint8_t entity_id;
        uint8_t message_length;
        std::string chat_message;
	} fields;
public:
	PlayerChat(InPacket& packet);
	PlayerChat(Fields& data);
	void read(InPacket& packet) override;
	void serialize(OutPacket& packet) override;
	void on_ack(Client&) override;
	void on_fail(Client&) override;
};
