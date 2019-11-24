#include "Config.h"


namespace config {
	// GameServer
	unsigned short GAME_PORT;
	unsigned int GAME_PROTOCOL;

	// Game
	bool DEBUG;
	unsigned int MAX_CONNECTIONS;

	// Client
	unsigned int TIMEOUT_MS;
	unsigned int RELIABLE_RESEND_MS;
}

std::string as_utf8(const char* str) {
	return str;
}

void config::load_config(const char* filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (!result) throw std::runtime_error("Failed to load config file.");

	auto game_server = doc.select_node("/Settings/GameServer").node();

	GAME_PORT = static_cast<unsigned short>(std::atoi(game_server.child_value("Port")));
	GAME_PROTOCOL = std::atoi(game_server.child_value("Protocol"));

	auto game = doc.select_node("/Settings/Game").node();
	
	DEBUG = as_utf8(game.child_value("Debug")) == "on";
	MAX_CONNECTIONS = std::atoi(game.child_value("MaxConnections"));

	auto client = doc.select_node("/Settings/Client").node();

	TIMEOUT_MS = std::atoi(client.child_value("Timeout"));
	RELIABLE_RESEND_MS = std::atoi(client.child_value("ReliableResend"));
}
