#include "GameServer.h"

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

/*void GameServer::send(unsigned char buffer[], OutPacket packet, Connection conn) {
	packet.setPacketLength();
	socket.sendPacket(buffer, packet.packet_length, conn.address, conn.port);
}*/

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
	// TODO: Circular buffers, all of this is temporary(?)

	unsigned char buffer[MAX_PACKET_SIZE]; // Make it a member?

	while (true) {
		InPacketInfo p_info = socket.receivePacket(buffer);

		if (p_info.buffer_size <= 0) { // No more packets to receive
			break;
		}

		// Do something with the received datagram

		try {
			InPacket in_packet = InPacket(buffer, p_info.buffer_size);

			long long connection = ((long long)p_info.sender_address << 32) + p_info.sender_port;

			switch (in_packet.packet_type) {
				case PacketType::Unreliable:			
					if (connections.find(connection) == connections.end()) {
						// Connection doesn't exist, simply ignore the packets (for now?)
					} else {
						connections[connection]->game->receiveCommand(connections[connection], in_packet);
					}
					break;
				case PacketType::Reliable:
					break;
				case PacketType::Control:
					ControlCmd command = in_packet.read<ControlCmd>();
					if (command == ControlCmd::ConnRequest) {

						bool game_found = false;

						unsigned char protocol = in_packet.read<unsigned char>();
						if (protocol == GAME_PROTOCOL) {
							if (connections.find(connection) == connections.end()) {
								// New connection, find game

								Client* result;
								for (Game* game : games) {
								
									result = game->connRequest();
									if (result != nullptr) {
										game_found = true;
										connections[connection] = result;
										break;
									}
								}
							} else {
								// Connection already exists, send an accept packet again
								game_found = true;
							}
						}

						// Send conn-accept/deny packet

						OutPacket out_packet = OutPacket(PacketType::Control, buffer);
						if (game_found) {
							out_packet.write(ControlCmd::ConnAccept);
						} else {
							out_packet.write(ControlCmd::ConnDeny);
						}

						send(buffer, out_packet, p_info.sender_address, p_info.sender_port);
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
