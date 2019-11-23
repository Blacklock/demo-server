#include "GameServer.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <stdint.h>


GameServer::GameServer(unsigned short port) {
	socket.create(port);
	createGame(); // Create a single game instance

	std::cout << "Starting DemoServer...\n";
	std::cout << "DEBUG: " << (config::DEBUG ? "On" : "Off") << "\n";

	startGameLoop();

	/*std::thread t(&GameServer::startGameLoop, this);
	while (true) { // get rid of this monstrosity
		std::cin;
	}*/
}

void GameServer::createGame() {
	games.emplace_back(std::make_unique<Game>(&socket));
}

void GameServer::startGameLoop() {
	using delta = std::chrono::duration<std::int64_t, std::ratio<1, 64>>; // Tickrate
	auto nextTick = std::chrono::steady_clock::now() + delta(1);

	std::unique_lock<std::mutex> lock(mtx);

	while (!stopGameLoop) {
		tick();

		control.wait_until(lock, nextTick, []{ return false; });
		nextTick += delta(1);
	}
}

void GameServer::tick() {
	// TIMER START >>>
	//auto t1 = std::chrono::high_resolution_clock::now();

	while (true) {
		InPacketInfo p_info = socket.receivePacket(buffer);

		if (p_info.buffer_size <= 0) { // No more packets to receive
			break;
		}

		// Process the received datagram
		try {
			InPacket in_packet = InPacket(buffer, p_info.buffer_size);

			long long connection = ((long long)p_info.sender_address << 32) + p_info.sender_port;

			// Find the connection
			bool connection_exists = false;
			std::unordered_map<long long, std::unique_ptr<Client>>::iterator conn;
			for (auto&& game : games) {
				conn = game->connections.find(connection);
				connection_exists = conn != game->connections.end();
				if (connection_exists) break;
			}

			switch (in_packet.packet_type) {
				case PacketType::Unreliable:
					if (!connection_exists) {
						// Connection doesn't exist, simply ignore the packets (for now?)
						std::cout << "Connection does not exist.\n";
					} else {
						conn->second->game->receiveMessage(*conn->second, in_packet);
					}
					break;
				case PacketType::Reliable:
					break;
				case PacketType::Control:
					ControlCmd command = in_packet.read<ControlCmd>();

					if (command == ControlCmd::ConnRequest) {
						bool game_found = false;
						uint32_t protocol = in_packet.read<uint32_t>();

						if (protocol == config::GAME_PROTOCOL) {
							if (!connection_exists) {
								// New connection, find a game
								for (auto&& game : games) {
									game_found = game->connectClient(connection, p_info);
									if (game_found) break;
								}
							} else {
								// Connection already exists, send an accept packet again
								std::cout << "Accepted reconnection\n";
								game_found = true;
							}

							// Send conn response packet
							OutPacket out_packet = OutPacket(PacketType::Control, buffer);
							out_packet.write(game_found ? ControlCmd::ConnAccept : ControlCmd::ConnDeny);
							out_packet.setPacketLength();

							socket.sendPacket(
								out_packet.buffer, out_packet.packet_length, 
								p_info.sender_address, p_info.sender_port
							);
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
	}

	// Loop over all clients, check if they have timed out, and send them snapshots
	// TODO: Multithreading
	for (auto&& game : games) {
		game->sendTickMessages();
	}

	// TIMER END >>>
	/*auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << duration << " microseconds\n";*/
}
