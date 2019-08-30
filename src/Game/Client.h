#pragma once

#include "../Utils/AckBuffer.h"

#include <chrono>


class Game;

class Client {
public:
	const unsigned ip;
	const unsigned port;

	Game* game;
	AckBuffer sequences = AckBuffer();
	unsigned short server_sequence = 1; // Sequences start at 1
private:
	std::chrono::steady_clock::time_point last_received;
public:
	Client(Game* client_game, unsigned long ip, unsigned short port);
	void bump();
};
