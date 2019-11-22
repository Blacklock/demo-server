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
	static const unsigned int MAX_PACKET_SIZE = 512;
	unsigned char buffer[MAX_PACKET_SIZE];

	SnapshotManager snapshot_manager = SnapshotManager();
	uint8_t connections_num = 0;
public:
	Game(Socket* socket);

	int connRequest();
	void connectClient(Client& client);
	void disconnectClient(Client& client);

	void receiveMessage(Client& client, InPacket& packet);
	void sendMessage(Client& client, OutPacket& packet);
	void sendSnapshot(Client& client);
};
