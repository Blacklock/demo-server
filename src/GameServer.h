#pragma once

#include "Socket.h"
#include "Game/Packet/InPacket.h"
#include "Game/Packet/OutPacket.h"
#include "Game/Client.h"
#include "Game/Snapshot/Snapshot.h"
#include "Game/Game.h"
#include "Config.h"

#include <memory>
#include <vector>
#include <condition_variable>


class GameServer {
public:
	static const unsigned int MAX_PACKET_SIZE = 512;
	unsigned char buffer[MAX_PACKET_SIZE]; // use pointers
private:
	Socket socket = Socket(MAX_PACKET_SIZE);
	std::condition_variable control;
	std::mutex mtx;
	bool stopGameLoop = false;

	std::vector<std::unique_ptr<Game>> games;
public:
	GameServer(unsigned short port);

	void createGame();
private:
	void startGameLoop();
	void tick();
	bool processPacket();
};
