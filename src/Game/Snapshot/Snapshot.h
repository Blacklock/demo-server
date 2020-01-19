#pragma once

#include "../Packet/InPacket.h"
#include "../Entity/Entity.h"

#include <stdint.h>
#include <memory>
#include <unordered_map>


class Snapshot {
public:
	const unsigned short id; // Snapshot packet ID
	std::unordered_map<unsigned char, std::shared_ptr<Entity>> entities; // Entity IDs/states
public:
	Snapshot(unsigned short id);
};
