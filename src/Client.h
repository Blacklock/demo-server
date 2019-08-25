#pragma once

#include "Utils/AckBuffer.h"

#include <chrono>


class Game;

class Client {
public:
	Game* game;

	//CircularBuffer<unsigned short> client_sequences = CircularBuffer<unsigned short>(5);
	AckBuffer client_sequences = AckBuffer();

	double pos_x;
	double pos_y;
private:
	std::chrono::steady_clock::time_point last_received;
public:
	Client(Game* client_game);
	void bump();
};
