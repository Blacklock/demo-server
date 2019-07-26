#pragma once

#include "Socket.h"

#include <condition_variable>


class GameServer {
private:
	Socket socket;

	std::condition_variable control;
	std::mutex mtx;
	bool stopGameLoop;
public:
	GameServer(unsigned short port);
	~GameServer();

	void send(const char packet[], const char destIp[46], unsigned short port);
private:
	void startGameLoop();
	void tick();
};
