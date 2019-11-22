#pragma once

#include "pugixml.hpp"
#include <iostream>


namespace config {
	// GameServer
	extern unsigned short GAME_PORT;
	extern unsigned int GAME_PROTOCOL;

	// Game
	extern bool DEBUG;
	extern unsigned int MAX_CONNECTIONS;

	// Client
	extern unsigned int TIMEOUT_MS;

	void load_config(const char* filename);
}