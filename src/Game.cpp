#include "Game.h"
#include "GameServer.h"
#include "Game/Snapshot.h"

#include <iostream>


Game::Game(GameServer* gameServer) :
	server(gameServer) {}

Client* Game::connRequest(unsigned long ip, unsigned short port) {
	if (connections < MAX_CONNECTIONS) {
		// Once we implement disconnection, the ID system will be improved
		Client* client = new Client(this, connections, ip, port);

		clients[connections] = client;
		connections++;

		return client;
	}

	return nullptr;
}

void Game::receiveCommand(Client& client, InPacket packet) {
	// Receive an unreliable command
	UnreliableCmd command = packet.read<UnreliableCmd>();

	switch (command) {
		case UnreliableCmd::Snapshot: // rename to PlayerData or something like that
			// Update master game state

			break;
		default:
			throw std::invalid_argument("Unknown command.");
	}

	client.sequences.put(packet.packet_sequence);
}

void Game::sendCommand(Client& client, OutPacket packet) {
	// Set the headers
	packet.setHeaders(
		client.server_sequence, 
		client.sequences.empty ? (unsigned short)0 : client.sequences.last_sequence,
		client.sequences.ack_bitfield
	);

	// Increase our sequence by one
	client.server_sequence++;

	server->send(packet.buffer, packet, client.ip, client.port);
}
