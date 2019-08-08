#include "Game.h"
#include "GameServer.h"

#include <iostream>


Game::Game(GameServer* gameServer) {
	server = gameServer;
}

bool Game::connRequest(unsigned long address, unsigned short port) {
	if (connections < MAX_CONNECTIONS) {
		clients[connections] = new Client(address, port);
		connections += 1;
		return true;
	}

	return false;
}
