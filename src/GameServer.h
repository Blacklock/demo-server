#pragma once

#include "Socket.h"
#include "Game/Packet/InPacket.h"
#include "Game/Packet/OutPacket.h"
#include "Game/Client.h"
#include "Game/Snapshot.h"
#include "Game.h"

#include <vector>
#include <condition_variable>
#include <unordered_map>


class GameServer {
public:
	static const unsigned int GAME_PROTOCOL = 100100;
	static const unsigned int MAX_PACKET_SIZE = 512;
private:
	Socket socket = Socket(MAX_PACKET_SIZE);
	std::condition_variable control;
	std::mutex mtx;
	bool stopGameLoop = false;

	std::vector<Game*> games;
	std::unordered_map<long long, Client*> connections;
public:
	GameServer(unsigned short port);
	~GameServer();

	void createGame();
	void send(unsigned char buffer[], OutPacket packet, unsigned long destIp, unsigned short port);
private:
	void startGameLoop();
	void tick();
};
