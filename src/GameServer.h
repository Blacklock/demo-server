#pragma once

#include "Socket.h"
#include "Packet.h"
#include "Client.h"
#include "Game.h"

#include <vector>
#include <condition_variable>
#include <unordered_map>


class GameServer {
public:
	static const unsigned char GAME_PROTOCOL = 100;
	static const unsigned int MAX_PACKET_SIZE = 256;
private:
	Socket socket = Socket(MAX_PACKET_SIZE);
	std::condition_variable control;
	std::mutex mtx;
	bool stopGameLoop;

	std::vector<Game*> games;
	std::unordered_map<long long, Client*> connections;
public:
	GameServer(unsigned short port);
	~GameServer();

	void createGame();
	void send(unsigned char buffer[], OutPacket packet, unsigned long destIp, unsigned short port);
	//void send(unsigned char buffer[], OutPacket packet, Conne client);
private:
	void startGameLoop();
	void tick();
};
