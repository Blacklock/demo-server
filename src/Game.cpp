#include "Game.h"
#include "GameServer.h"
#include "Game/Snapshot.h"

#include <iostream>
#include <stdint.h>


Snapshot Game::dummy_snapshot = Snapshot();

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
	// Do something with these later
	/*unsigned short sequence = packet.read<unsigned short>();
	unsigned short ack = packet.read<unsigned short>();
	uint32_t ack_bitfield = packet.read<uint32_t>();*/

	std::cout << "Sequence " << packet.packet_sequence << 
	" Ack " << packet.packet_ack << " AckBitfield " << packet.ack_bitfield << "\n";

	// Receive an unreliable command
	UnreliableCmd command = packet.read<UnreliableCmd>();

	switch (command) {
		case UnreliableCmd::PlayerState: // rename to PlayerData or something like that
			// Update master game state
			snapshot_manager.updatePlayerState(packet, client);
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
