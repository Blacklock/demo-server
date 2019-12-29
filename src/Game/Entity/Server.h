#pragma once

#include "Entity.h"
#include "../Packet/InPacket.h"
#include "../Packet/OutPacket.h"

#include <memory>


class Server : public Entity { // Global entity for gamestate
public:
	struct State {
		uint8_t status;
	};

	enum class Fields : unsigned char {
		Status, // 0=stopped, 1=paused, 2=running
		End
	};

	union ModFields {
		struct {
			bool status : 1;
			bool empty : 7;
		} fields;
		unsigned char raw;
	};

	State entity_state;
private:
	static const State dummy_state;
public:
	Server();
	Server(State& state);
	void read(InPacket& packet) override;
	void serialize(OutPacket& packet) override;
	void serialize(OutPacket& packet, Entity& last_entity) override;
	void serialize(OutPacket& packet, const State& last_state);
};