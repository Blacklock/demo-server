#include "GameServer.h"

#include <iostream>
#include <thread>
#include <chrono>


GameServer::GameServer(unsigned short port) {
	stopGameLoop = false;

	socket.create(port);

	//std::thread t{&GameServer::startGameLoop, this};
	std::thread t(&GameServer::startGameLoop, this);

	while (true) { // Remove this
		std::cin;
	}
}

GameServer::~GameServer() {}

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

	while (true) {
		InPacketInfo packet_info = socket.receivePacket(buffer);

		if (packet_info.buffer_size <= 0) { // No more packets to receive
			break;
		}

		// Do something with the received datagram

		try {
			Packet in_packet = Packet(buffer, packet_info.buffer_size);

			std::cout << "Packet type: " << static_cast<std::underlying_type_t<PacketType>>(in_packet.packet_type) << "\n";
			std::cout << "Packet length: " << in_packet.packet_length << "\n";

			switch (in_packet.packet_type) {
				case PacketType::Unreliable:
				case PacketType::Reliable:
					break;
				case PacketType::Control:
					switch (in_packet.packet_cmd.c_cmd) {
						case ControlCmd::ConnRequest:
							Packet out_packet = Packet(buffer, PacketType::Control);
							unsigned short number = 1;
							out_packet.append(number);

							std::cout << "PORT [" << packet_info.sender_port << "]\n";
							
							send(buffer, out_packet, packet_info.sender_address, packet_info.sender_port);

							break;
					}
					break;
			}

		} catch (const std::invalid_argument ex) {
			std::cerr << ex.what();
			std::cout << "\n========\n";
		}
		
		for (int i = 0; i < packet_info.buffer_size; i++) {
			std::cout << buffer[i];
		}

		std::cout << "\n";
	}
}

void GameServer::send(unsigned char buffer[], Packet packet, unsigned long destIp, unsigned short port) { // const char destIp[46]
	packet.setPacketLength();
	socket.sendPacket(buffer, packet.packet_length, destIp, port);
}
