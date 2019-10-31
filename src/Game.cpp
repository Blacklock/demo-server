#include "Game.h"
#include "Game/Packet/Packet.h"
#include "GameServer.h"
#include "Game/Snapshot/Snapshot.h"
#include "Config.h"

#include <iostream>
#include <stdint.h>


/*Game::Game(GameServer* gameServer) :
	server(gameServer) {}*/

Game::Game(Socket* socket) : socket(socket) {}

int Game::connRequest() {
	if (connections_num < config::MAX_CONNECTIONS) {
		connections_num++;
		return connections_num - 1;
	}
	return -1;
}

void Game::connectClient(Client& client) {
	snapshot_manager.addPlayer(client); // Create a PlayerState for the new client
}

void Game::disconnectClient(Client& client) {
	// TODO(?): Send a PlayerLeave packet
	snapshot_manager.removePlayer(client);
}

void Game::receiveCommand(Client& client, InPacket& packet) {
	client.bump(); // Bump the client's last_received timer

	// Receive an unreliable command
	UnreliableCmd command = packet.read<UnreliableCmd>();
	switch (command) {
		case UnreliableCmd::PlayerData: // rename to PlayerData or something like that
			// Update master game state
			snapshot_manager.updatePlayerState(client, packet);
			break;
		default:
			throw std::invalid_argument("Unknown command.");
	}

	client.sequences.put(packet.packet_ack); // Update ack

	// If newly received ack is larger than previous, update last received snapshot
	client.last_snapshot = client.sequences.last_sequence;
}

void Game::sendCommand(Client& client, OutPacket& packet) {
	packet.setHeaders(
		client.server_sequence, 
		client.sequences.empty ? (unsigned short)0 : client.sequences.last_sequence,
		client.sequences.ack_bitfield
	);

	client.server_sequence++; // Increase our sequence by one

	client.send(packet);
}

void Game::sendSnapshot(Client& client) {
	OutPacket ss_packet = OutPacket(PacketType::Unreliable, buffer);
	ss_packet.write(UnreliableCmd::Snapshot);

	snapshot_manager.writeSnapshot(client, ss_packet); // Write snapshot data

	sendCommand(client, ss_packet); // Send the packet
}
