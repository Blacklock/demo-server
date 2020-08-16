#pragma once

#include "Socket.h"
#include "Game/Packet/InPacket.h"
#include "Game/Packet/OutPacket.h"
#include "Game/Client.h"
#include "Game/Snapshot/Snapshot.h"
#include "Game/Game.h"
#include "Utils/ctpl.h"
#include "Config.h"

#include <boost/pool/object_pool.hpp>
#include <memory>
#include <vector>
#include <condition_variable>


class GameServer {
public:
	static const unsigned int MAX_PACKET_SIZE = 512;
	unsigned char buffer[MAX_PACKET_SIZE]; // use pointers
private:
	Socket socket = Socket(MAX_PACKET_SIZE);
	boost::object_pool<unsigned char[MAX_PACKET_SIZE]> buf_pool; // unnecessary??
	ctpl::thread_pool t_pool;

	std::vector<std::unique_ptr<Game>> games;

	std::condition_variable control;
	std::mutex mtx;
	bool stopGameLoop = false;
public:
	GameServer(unsigned short port);

	void createGame();
private:
	void startGameLoop();
	void tick();
	bool processPacket();
};
