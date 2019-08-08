#include "GameServer.h"
#include "Client.h"

#include <iostream>
#include <thread>
#include <chrono>


GameServer::GameServer(unsigned short port) {
	stopGameLoop = false;
	socket.create(port);

	std::thread t(&GameServer::startGameLoop, this);

	createGame();

	while (true) { // Remove this
		std::cin;
	}
}

GameServer::~GameServer() {}

void GameServer::createGame() {
	Game* game = new Game(this);
	games.push_back(game);
}

void GameServer::send(unsigned char buffer[], OutPacket packet, unsigned long destIp, unsigned short port) {
	packet.setPacketLength();
	socket.sendPacket(buffer, packet.packet_length, destIp, port);
}

void GameServer::send(unsigned char buffer[], OutPacket packet, Client client) {
	packet.setPacketLength();
	socket.sendPacket(buffer, packet.packet_length, client.address, client.port);
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

	unsigned char buffer[MAX_PACKET_SIZE]; // Make it a member?

	while (true) {
		InPacketInfo packet_info = socket.receivePacket(buffer);

		if (packet_info.buffer_size <= 0) { // No more packets to receive
			break;
		}

		// Do something with the received datagram

		try {
			InPacket in_packet = InPacket(buffer, packet_info.buffer_size);

			//int x = in_packet.read<int>();

			switch (in_packet.packet_type) {
			case PacketType::Unreliable:
			case PacketType::Reliable:
				break;
			case PacketType::Control:
				ControlCmd command = in_packet.read<ControlCmd>();
				if (command == ControlCmd::ConnRequest) {
					bool game_found = false;
					for (Game* game : games) {
						if (game->connRequest(packet_info.sender_address, packet_info.sender_port)) {
							game_found = true;
							break;
						}
					}

					OutPacket out_packet = OutPacket(PacketType::Control, buffer);
					if (game_found) {
						out_packet.write(ControlCmd::ConnAccept);
					} else {
						out_packet.write(ControlCmd::ConnDeny);
					}

					send(buffer, out_packet, packet_info.sender_address, packet_info.sender_port);
					break;
				}
			}
		} catch (const std::invalid_argument ex) {
			std::cerr << ex.what();
			std::cout << "\n========\n";
		}

		std::cout << "\n";
		// TODO: At the end of the tick, send out packets
	}
}
