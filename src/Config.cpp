#include "Config.h"


namespace config {
	// GameServer
	unsigned short GAME_PORT;
	unsigned int GAME_PROTOCOL;
	unsigned int TICKRATE;

	// Game
	bool DEBUG;
	bool MULTITHREADING;
	unsigned int MAX_CONNECTIONS;

	// Client
	unsigned int TIMEOUT_MS;
	unsigned int RELIABLE_RESEND_MS;
	unsigned int PACKET_LOG_COUNT;

	// Logger
	std::string SEVERITY;
	std::string LOG_FILE;

	// NetSim
	float IN_LOSS;
	float OUT_LOSS;
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
	TICKRATE = std::atoi(game_server.child_value("Tickrate"));

	auto game = doc.select_node("/Settings/Game").node();
	
	DEBUG = as_utf8(game.child_value("Debug")) == "true";
	MULTITHREADING = as_utf8(game.child_value("Debug")) == "true";
	MAX_CONNECTIONS = std::atoi(game.child_value("MaxConnections"));

	auto client = doc.select_node("/Settings/Client").node();

	TIMEOUT_MS = std::atoi(client.child_value("Timeout"));
	RELIABLE_RESEND_MS = std::atoi(client.child_value("ReliableResend"));
	PACKET_LOG_COUNT = std::atoi(client.child_value("PacketLogCount"));

	auto logger = doc.select_node("/Settings/Logger").node();

	SEVERITY = as_utf8(logger.child_value("Severity"));
	LOG_FILE = as_utf8(logger.child_value("LogFile"));

	auto net_sim = doc.select_node("/Settings/NetSim").node();

	IN_LOSS = std::stof(net_sim.child_value("InLoss"));
	OUT_LOSS = std::stof(net_sim.child_value("OutLoss"));
}
