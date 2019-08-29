#include "Game.h"
#include "GameServer.h"
#include "Game/Snapshot.h"

#include <iostream>


Game::Game(GameServer* gameServer) :
	server(gameServer),
	snapshot_manager(&server->dummy_snapshot) {}

Client* Game::connRequest() {
	if (connections < MAX_CONNECTIONS) {
		Client* client = new Client(this);

		clients[connections] = client;
		connections += 1;

		return client;
	}

	return nullptr;
}

void Game::receiveCommand(Client& client, InPacket packet) {
	UnreliableCmd command = packet.read<UnreliableCmd>();

	switch (command) {
		case UnreliableCmd::Snapshot:
			// work on this once we finish up with acks
			break;
		default:
			throw std::invalid_argument("Unknown command.");
	}

	client.client_sequences.put(packet.packet_sequence);
}

void Game::sendCommand(Client& client, OutPacket packet) {
	
}
