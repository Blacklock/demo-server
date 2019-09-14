#pragma once

#include "Game/Packet/InPacket.h"
#include "Game/Packet/OutPacket.h"
#include "Game/Client.h"
#include "Game/Snapshot/SnapshotManager.h"


class GameServer;

class Game {
private:
	static const unsigned int MAX_CONNECTIONS = 4;

	GameServer* server;
	static Snapshot dummy_snapshot;
	SnapshotManager snapshot_manager = SnapshotManager();
	Client* clients[MAX_CONNECTIONS] = {};
	unsigned char connections = 0;
public:
	Game(GameServer* gameServer);

	Client* connRequest(unsigned long ip, unsigned short port);
	void receiveCommand(Client& client, InPacket& packet);
	void sendCommand(Client& client, OutPacket& packet);
	void sendSnapshots();
};
