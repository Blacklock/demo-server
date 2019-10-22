#include "GameServer.h"

#include <iostream>
#include <stdint.h>
#include <thread>
#include <chrono>


GameServer::GameServer(unsigned short port) {
	socket.create(port);
	createGame(); // Create a single game instance
	startGameLoop();

	/*std::thread t(&GameServer::startGameLoop, this);
	while (true) { // get rid of this monstrosity
		std::cin;
	}*/
}

void GameServer::createGame() {
	games.emplace_back(std::make_unique<Game>(&socket));
	//games.emplace_back(new Game(&socket));
}

void GameServer::startGameLoop() {
	using delta = std::chrono::duration<std::int64_t, std::ratio<1, 64>>;
	auto nextTick = std::chrono::steady_clock::now() + delta(1);

	std::unique_lock<std::mutex> lock(mtx);

	while (!stopGameLoop) {
		mtx.unlock(); // why unlock/lock every time? rework the game loop code later

		tick();

		mtx.lock();

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

		// Do something with the received datagram

		try {
			InPacket in_packet = InPacket(buffer, p_info.buffer_size);

			long long connection = ((long long)p_info.sender_address << 32) + p_info.sender_port;

			std::unordered_map<long long, std::unique_ptr<Client>>::iterator conn = connections.find(connection);
			switch (in_packet.packet_type) {
				case PacketType::Unreliable:
					if (conn == connections.end()) {
						// Connection doesn't exist, simply ignore the packets (for now?)
						std::cout << "Connection does not exist.\n";
					} else {
						conn->second->game->receiveCommand(*conn->second, in_packet);
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

								for (auto&& game : games) {
									int client_id = game->connRequest();
									if (client_id != -1) { // Game isn't full
										//Client* client = new Client(game.get(), (unsigned char)client_id, p_info.sender_address, p_info.sender_port);
										connections[connection] = std::make_unique<Client>(
											game.get(), (unsigned char)client_id, p_info.sender_address, p_info.sender_port
										);

										game_found = true;
										std::cout << "Found an open game for new connection\n";
										break;
									}
								}
							} else {
								// Connection already exists, send an accept packet again
								std::cout << "Connection already exists\n";
								game_found = true;
							}

							// Send conn response packet

							std::cout << "Sending connection response packet\n";

							OutPacket out_packet = OutPacket(PacketType::Control, buffer);
							out_packet.write(game_found ? ControlCmd::ConnAccept : ControlCmd::ConnDeny);

							connections[connection]->send(out_packet);
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
		/*for (size_t i = 0; i < games.size(); ++i) {
			games[i]->sendSnapshots();
		}*/

		// Loop over all clients and send them snapshots
		// Currently we only send snapshots if we received any packets
		std::unordered_map<long long, std::unique_ptr<Client>>::iterator conn;
		for (conn = connections.begin(); conn != connections.end(); ++conn) {
			conn->second->game->sendSnapshot(*conn->second);
		}

		std::cout << "\n";
	}

	// TIMER END >>>
	/*auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << duration << " microseconds\n";*/
}
