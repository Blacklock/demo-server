#pragma once

#include "pugixml.hpp"
#include <iostream>
#include <string>


namespace config {
	// GameServer
	extern unsigned short GAME_PORT;
	extern unsigned int GAME_PROTOCOL;

	// Game
	extern bool DEBUG;
	extern bool MULTITHREADING;
	extern unsigned int MAX_CONNECTIONS;

	// Client
	extern unsigned int PACKET_LOSS_COUNT;
	extern unsigned int TIMEOUT_MS;
	extern unsigned int RELIABLE_RESEND_MS;

	// Logger
	extern std::string SEVERITY;
	extern std::string LOG_FILE;

	// NetSim
	extern float IN_LOSS;
	extern float OUT_LOSS;

	void load_config(const char* filename);
}
