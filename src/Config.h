#pragma once

#include "pugixml.hpp"
#include <iostream>


namespace config {
	// GameServer
	extern unsigned short GAME_PORT;
	extern unsigned int GAME_PROTOCOL;

	// NetSim
	extern float IN_LOSS;
	extern float OUT_LOSS;

	// Game
	extern bool DEBUG;
	extern unsigned int MAX_CONNECTIONS;

	// Client
	extern unsigned int PACKET_LOSS_COUNT;
	extern unsigned int TIMEOUT_MS;
	extern unsigned int RELIABLE_RESEND_MS;

	void load_config(const char* filename);
}
