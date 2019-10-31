#include "Config.h"


namespace config {
	// GameServer
	bool DEBUG;
	unsigned short GAME_PORT;
	unsigned int GAME_PROTOCOL;
	unsigned int MAX_PACKET_SIZE;

	// Game
	unsigned int MIN_CONNECTIONS;
	unsigned int MAX_CONNECTIONS;

	// Client
	bool TIMEOUT_ENABLED;
	unsigned int TIMEOUT_MS;
}

std::string as_utf8(const char* str) { 
	return str;
}

void config::load_config(const char* filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (!result) throw std::runtime_error("Failed to load config file.");

	auto game_server = doc.select_node("/Settings/GameServer").node();

	DEBUG = as_utf8(game_server.child_value("Debug")) == "on";
	GAME_PORT = static_cast<unsigned short>(std::atoi(game_server.child_value("Port")));
	GAME_PROTOCOL = std::atoi(game_server.child_value("Protocol"));
	MAX_PACKET_SIZE = std::atoi(game_server.child_value("MaxPacketSize"));

	auto game = doc.select_node("/Settings/Game").node();
	
	MIN_CONNECTIONS = std::atoi(game.child_value("MinConnections"));
	MAX_CONNECTIONS = std::atoi(game.child_value("MaxConnections"));

	auto client = doc.select_node("/Settings/Client").node();

	TIMEOUT_MS = std::atoi(client.child_value("Timeout"));
	TIMEOUT_ENABLED = TIMEOUT_MS <= 0;
}
