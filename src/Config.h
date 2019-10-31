#pragma once

#include "pugixml.hpp"
#include <iostream>


namespace config {
	// GameServer
	extern bool DEBUG;
	extern unsigned short GAME_PORT;
	extern unsigned int GAME_PROTOCOL;
	extern unsigned int MAX_PACKET_SIZE;

	// Game
	extern unsigned int MIN_CONNECTIONS;
	extern unsigned int MAX_CONNECTIONS;

	// Client
	extern bool TIMEOUT_ENABLED;
	extern unsigned int TIMEOUT_MS;

	void load_config(const char* filename);
}
