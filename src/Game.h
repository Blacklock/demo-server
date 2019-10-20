#pragma once

#include "Socket.h"
#include "Game/Packet/InPacket.h"
#include "Game/Packet/OutPacket.h"
#include "Game/Client.h"
#include "Game/Snapshot/SnapshotManager.h"

#include <memory>
#include <vector>
#include <stdint.h>


class GameServer;

class Game {
public:
	Socket* socket;
private:
	static const unsigned int MIN_CONNECTIONS = 4; // used for vector
	static const unsigned int MAX_CONNECTIONS = 4;
	static const unsigned int MAX_PACKET_SIZE = 512;

	unsigned char buffer[MAX_PACKET_SIZE];

	SnapshotManager snapshot_manager = SnapshotManager();
	uint8_t connections_num = 0;
public:
	//Game(GameServer* gameServer);
	Game(Socket* socket);

	int connRequest();
	void receiveCommand(Client& client, InPacket& packet);
	void sendCommand(Client& client, OutPacket& packet);
	void sendSnapshot(Client& client);
};
