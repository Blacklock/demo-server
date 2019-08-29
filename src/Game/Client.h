#pragma once

#include "../Utils/AckBuffer.h"

#include <chrono>


class Game;

class Client {
public:
	Game* game;
	AckBuffer client_sequences = AckBuffer();
private:
	std::chrono::steady_clock::time_point last_received;
public:
	Client(Game* client_game);
	void bump();
};
