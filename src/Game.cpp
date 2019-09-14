#include "Game.h"
#include "Game/Packet/Packet.h"
#include "GameServer.h"
#include "Game/Snapshot/Snapshot.h"

#include <iostream>
#include <stdint.h>


Snapshot Game::dummy_snapshot = Snapshot(0); // this has to be reworked

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

void Game::receiveCommand(Client& client, InPacket& packet) {
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

void Game::sendCommand(Client& client, OutPacket& packet) {
	// Set the headers
	packet.setHeaders(
		client.server_sequence, 
		client.sequences.empty ? (unsigned short)0 : client.sequences.last_sequence,
		client.sequences.ack_bitfield
	);

	// Increase our sequence by one
	client.server_sequence++;

	server->send(packet, client.ip, client.port);
}

void Game::sendSnapshots() {
	// Iterate over all clients
	for (unsigned char i = 0; i < connections; ++i) {
		OutPacket ss_packet = OutPacket(PacketType::Unreliable, server->buffer);
		ss_packet.write(UnreliableCmd::Snapshot);

		snapshot_manager.writeSnapshot(ss_packet, *clients[i]); // Write snapshot data

		sendCommand(*clients[i], ss_packet); // Send the packet
	}
}
