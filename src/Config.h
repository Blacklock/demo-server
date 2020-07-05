#pragma once

#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <stdint.h>


namespace config {
	// GameServer
	extern std::string VERSION;
	extern unsigned short GAME_PORT;
	extern unsigned int GAME_PROTOCOL;
	extern unsigned int TICKRATE;

	// Game
	extern bool DEBUG;
	extern bool MULTITHREADING;
	extern unsigned int MAX_CONNECTIONS;

	// Client
	extern unsigned int TIMEOUT_MS;
	extern unsigned int RELIABLE_RESEND_MS;
	extern unsigned int PACKET_LOG_COUNT;

	// Logger
	extern std::string SEVERITY;
	extern std::string LOG_FILE;

	// NetSim
	extern float IN_LOSS;
	extern float OUT_LOSS;

	void load_config(const char* filename);
}
