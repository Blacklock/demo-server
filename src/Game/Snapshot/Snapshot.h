#pragma once

#include "../Packet/InPacket.h"
#include "../Entity/Entity.h"

#include <stdint.h>
#include <memory>
#include <unordered_map>


class Snapshot {
public:
	// Snapshot packet ID
	const unsigned short id;
	// Map of entity IDs/states
	std::unordered_map<unsigned char, std::shared_ptr<Entity>> entities;

	// TODO: instead of storing the same PlayerState data for every single Snapshot
	// object, we will simply pass around shared pointers. Once the PlayerState
	// isn't being used anywhere anymore, it'll get deleted by itself.
public:
	Snapshot(unsigned short id);
};
