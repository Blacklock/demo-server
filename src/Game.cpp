#include "Game.h"
#include "GameServer.h"
#include "Game/Snapshot.h"

#include <iostream>


Game::Game(GameServer* gameServer) :
	server(gameServer),
	snapshot_manager() {}

Client* Game::connRequest() {
	if (connections < MAX_CONNECTIONS) {
		Client* client = new Client(this);

		clients[connections] = client;
		connections += 1;

		return client;
	}

	return nullptr;
}

void Game::receiveCommand(Client* client, InPacket packet) {
	UnreliableCmd command = packet.read<UnreliableCmd>();

	switch (command) {
		/*case UnreliableCmd::PosUpdate:
			int32_t x = packet.read<int32_t>();
			int32_t y = packet.read<int32_t>();
			
			client->pos_x = (double)x / 1000;
			client->pos_y = (double)y / 1000;

			std::cout << "X: " << client->pos_x << " Y: " << client->pos_y << "\n";

			OutPacket ping_packet = OutPacket(PacketType::Control, server->buffer);
			
			// temp command, remove this later.

			break;*/
		case UnreliableCmd::Snapshot:
			{ Snapshot snapshot = Snapshot(nullptr); } // temp
			
			break;
		default:
			throw std::invalid_argument("Unknown command.");
	}

	client->client_sequences.put(packet.packet_sequence);
}

void Game::sendCommand(Client* client, OutPacket packet) {
	
}
