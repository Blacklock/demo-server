#pragma once

#include "../Utils/AckBuffer.h"
#include "Snapshot.h"

#include <chrono>


class Game;

class Client {
public:
	const unsigned char id;
	const unsigned long ip;
	const unsigned short port;

	Game* game;
	unsigned short server_sequence = 1; // Sequences start at 1
	AckBuffer sequences = AckBuffer();

	//Snapshot* snapshots[32];
private:
	std::chrono::steady_clock::time_point last_received;
public:
	Client(Game* client_game, unsigned char id, unsigned long ip, unsigned short port);
	void bump();
};
