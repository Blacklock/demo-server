#pragma once

#include "Packet.h"
#include "Socket.h"
#include "Client.h"

#include <condition_variable>


class GameServer {
public:
	static const unsigned char GAME_PROTOCOL = 100;
	static const unsigned int MAX_PACKET_SIZE = 256;
	static const unsigned int MAX_CONNECTIONS = 4;
private:
	Socket socket = Socket(MAX_PACKET_SIZE);
	Client* clients[MAX_CONNECTIONS] = {};
	std::condition_variable control;
	std::mutex mtx;
	bool stopGameLoop;
public:
	GameServer(unsigned short port);
	~GameServer();
private:
	void startGameLoop();
	void tick();
	void send(unsigned char buffer[], Packet packet, unsigned long destIp, unsigned short port);
};
