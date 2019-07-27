#include "GameServer.h"
#include "Packet.h"

#include <iostream>
#include <thread>
#include <chrono>


GameServer::GameServer(unsigned short port) {
	stopGameLoop = false;

	socket.create(port);

	//std::thread t{&GameServer::startGameLoop, this};
	std::thread t(&GameServer::startGameLoop, this);

	while (true) {
		std::cin;
	}

	/*{
		std::lock_guard<std::mutex> lk(mtx);
		stopGameLoop = true;
	}
	
	t.join();*/

	/*
	using namespace std::chrono_literals;
	while (true) { // work on this abomination later

		std::this_thread::sleep_for(1s / 64); // 64 tick

		tick();
	}*/
}

GameServer::~GameServer() {}

void GameServer::send(const char packet[], const char destIp[46], unsigned short port) {
	socket.sendPacket(packet, destIp, port);
}

void GameServer::startGameLoop() {
	using delta = std::chrono::duration<std::int64_t, std::ratio<1, 64>>;
	auto nextTick = std::chrono::steady_clock::now() + delta(1);

	std::unique_lock<std::mutex> lock(mtx);

	while (!stopGameLoop) {
		mtx.unlock();

		tick();

		mtx.lock();

		control.wait_until(lock, nextTick, []{ return false; });
		nextTick += delta(1);
	}
}

void GameServer::tick() {
	// TODO: Circular buffers, all of this is temporary

	unsigned char buffer[MAX_PACKET_SIZE];
	int buffer_size;

	while (true) {
		buffer_size = socket.receivePacket(buffer);

		if (buffer_size <= 0) { // No more packets to receive
			break;
		}

		// Do something with received datagram

		Packet received_packet = Packet(buffer, buffer_size);
		
		for (int i = 0; i < buffer_size; i++) {
			std::cout << buffer[i];
		}

		std::cout << "\n";
	}
}
