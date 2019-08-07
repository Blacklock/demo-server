#pragma once

#include "Client.h"


class GameServer;

class Game {
private:
	static const unsigned int MAX_CONNECTIONS = 4;

	GameServer* server;
	Client* clients[MAX_CONNECTIONS] = {};
	unsigned int connections = 0;
public:
	Game(GameServer* gameServer);

	bool connRequest(unsigned long address, unsigned short port);
};
