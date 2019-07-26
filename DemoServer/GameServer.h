#pragma once

#include "Socket.h"

#include <functional>


class GameServer {
private:
	Socket socket;
public:
	GameServer(unsigned short port);
	~GameServer();

	void startGameLoop(std::function<void(void)> func, unsigned int interval);
	void send(const char packet[], const char destIp[46], unsigned short port);
private:
	void tick();
};
