#include "Game.h"
#include "Game/Packet/Packet.h"
#include "GameServer.h"
#include "Game/Snapshot/Snapshot.h"
#include "Game/Message/PlayerDisconnect.h"
#include "Config.h"

#include <iostream>
#include <stdint.h>


/*Game::Game(GameServer* gameServer) :
	server(gameServer) {}*/

Game::Game(Socket* socket) : socket(socket) {}

/*int Game::connRequest() {
	if (connections_num < config::MAX_CONNECTIONS) {
		connections_num++;
		return connections_num - 1;
	}
	return -1;
}*/

bool Game::connectClient(long long connection, InPacketInfo p_info) {
	if (connections_num >= config::MAX_CONNECTIONS) return false;

	connections[connection] = std::make_unique<Client>(
		this, connections_num, p_info.sender_address, p_info.sender_port
	);

	snapshot_manager.addPlayer(*connections[connection]); // Create a PlayerState for the new client

	connections_num++;
	return true;
}

void Game::disconnectClient(Client& client) {
	// TODO: Send a PlayerDisconnect packet
	// all_clients => client.reliable_queue.add(playerdisconnect,id);
	OutPacket pdc_packet = OutPacket(PacketType::Reliable, buffer);

	// sendSnapshot snippet
	pdc_packet.write(UnreliableCmd::Snapshot);
	snapshot_manager.writeSnapshot(client, pdc_packet); // Write snapshot data

	PlayerDisconnect::Fields pdc_fields{ client.id };
	PlayerDisconnect pdc_message = PlayerDisconnect(pdc_fields);

	// TODO: send pdc_message to all clients...

	snapshot_manager.removePlayer(client);
}

void Game::receiveMessage(Client& client, InPacket& packet) {
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

void Game::sendMessage(Client& client, OutPacket& packet) {
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

	sendMessage(client, ss_packet); // Send the packet
}

void sendReliable(Client& client) {
	client;
}
