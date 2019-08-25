#pragma once

#include "Utils/CircularBuffer.h"

#include <chrono>


class Game;

class Client {
public:
	Game* game;

	//CircularBuffer<unsigned short> client_sequences = CircularBuffer<unsigned short>(5);

	unsigned short server_sequence;
	unsigned short client_sequence;

	double pos_x;
	double pos_y;
private:
	std::chrono::steady_clock::time_point last_received;
public:
	Client(Game* client_game);
	void bump();
};
