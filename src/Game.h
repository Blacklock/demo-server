#pragma once

#include "Packet.h"
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

	Client* connRequest();
	void receiveCommand(Client* client, InPacket packet);
};
