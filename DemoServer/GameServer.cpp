#include "GameServer.h"

#include <iostream>
#include <thread>
#include <chrono>


GameServer::GameServer(unsigned short port) {
	using namespace std::chrono_literals;

	socket.create(port);

	while (true) { // work on this abomination later

		std::this_thread::sleep_for(1s / 64); // 64 tick, supposedly

		tick();
	}
}

GameServer::~GameServer() {}

void GameServer::send(const char packet[], const char destIp[46], unsigned short port) {
	socket.sendPacket(packet, destIp, port);
}

void GameServer::tick() {
	socket.receivePackets();
}
