#include "GameServer.h"

#include <iostream>
#include <stdint.h>
#include <thread>
#include <chrono>


GameServer::GameServer(unsigned short port) {
	socket.create(port);
	std::thread t(&GameServer::startGameLoop, this);
	createGame(); // Create a single game instance

	while (true) { // Deal with this!!
		std::cin;
	}
}

GameServer::~GameServer() {}

void GameServer::createGame() {
	games.push_back(std::make_unique<Game>(this));
}

void GameServer::send(OutPacket packet, unsigned long destIp, unsigned short port) {
	packet.setPacketLength();
	socket.sendPacket(buffer, packet.packet_length, destIp, port);
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
	// TIMER START >>>
	//auto t1 = std::chrono::high_resolution_clock::now();

	//unsigned char buffer[MAX_PACKET_SIZE]; // Make it a member?

	while (true) {
		InPacketInfo p_info = socket.receivePacket(buffer);

		if (p_info.buffer_size <= 0) { // No more packets to receive
			break;
		}

		// Do something with the received datagram

		try {
			InPacket in_packet = InPacket(buffer, p_info.buffer_size);

			long long connection = ((long long)p_info.sender_address << 32) + p_info.sender_port;
			std::cout << "Received a packet " << (unsigned char)in_packet.packet_type << "\n";
			switch (in_packet.packet_type) {
				case PacketType::Unreliable:			
					if (connections.find(connection) == connections.end()) {
						// Connection doesn't exist, simply ignore the packets (for now?)
						std::cout << "Connection does not exist.\n";
					} else {
						connections[connection]->game->receiveCommand(*connections[connection], in_packet);
					}
					break;
				case PacketType::Reliable:
					break;
				case PacketType::Control:
					ControlCmd command = in_packet.read<ControlCmd>();

					if (command == ControlCmd::ConnRequest) {
						bool game_found = false;

						uint32_t protocol = in_packet.read<uint32_t>();
						if (protocol == GAME_PROTOCOL) {
							if (connections.find(connection) == connections.end()) {
								// New connection, find game

								//for (Game* game : games) {
								for (auto&& game : games) {
									try {
										Client result = game->connRequest(p_info.sender_address, p_info.sender_port);

										game_found = true;
										connections[connection] = &result;
										break;
									} catch (const std::exception& ex) { (void)ex; } // Game is full
								}
							} else {
								// Connection already exists, send an accept packet again
								game_found = true;
							}

							// Send conn response packet

							std::cout << "done!\n";

							OutPacket out_packet = OutPacket(PacketType::Control, buffer);
							out_packet.write(game_found ? ControlCmd::ConnAccept : ControlCmd::ConnDeny);
							send(out_packet, p_info.sender_address, p_info.sender_port);
						} else {
							throw std::invalid_argument("Unknown protocol.");
						}

						break;
					} else {
						throw std::invalid_argument("Invalid control command.");
					}
			}
		} catch (const std::invalid_argument ex) {
			std::cerr << ex.what();
		}

		// Loop over all games and send snapshots to all clients
		for (size_t i = 0; i < games.size(); ++i) {
			games[i]->sendSnapshots();
		}

		std::cout << "\n";
	}

	// TIMER END >>>
	/*auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << duration << " microseconds\n";*/
}
