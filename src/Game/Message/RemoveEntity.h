#pragma once

#include "ReliableMessage.h"
#include "../Client.h"
#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>
#include <stdint.h>


class RemoveEntity : public ReliableMessage {
public:
	struct Fields {
		uint8_t entity_id;
	} fields;
public:
	RemoveEntity(InPacket& packet);
	RemoveEntity(Fields& data);
	void read(InPacket& packet) override;
	void serialize(OutPacket& packet) override;
	void onAck(Client& client) override;
	void onFail(Client& client) override;
};
