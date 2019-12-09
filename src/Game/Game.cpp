#include "Game.h"
#include "Packet/Packet.h"
#include "../GameServer.h"
#include "Snapshot/Snapshot.h"
#include "Message/PlayerDisconnect.h"
#include "../Config.h"

#include <iostream>
#include <stdint.h>
#include <thread>


Game::Game(Socket* socket) : socket(socket) {}

bool Game::connectClient(long long connection, InPacketInfo p_info) {
	if (connections_num >= config::MAX_CONNECTIONS) return false;
	std::cout << "Accepted connection " << static_cast<int>(connections_num) << "\n";

	connections[connection] = std::make_unique<Client>(
		this, connections_num, p_info.sender_address, p_info.sender_port
	);

	snapshot_manager.addPlayer(*connections[connection]); // Create a PlayerState for the new client

	connections_num++;

	// temp v
	/*std::cout << "pushing reliable queue message playerdisconnect\n";
	PlayerDisconnect::Fields pdc_fields{ 15 };
	PlayerDisconnect pdc_message = PlayerDisconnect(pdc_fields);
	auto message = std::make_shared<PlayerDisconnect>(pdc_message);
	connections[connection]->reliable_queue.push(message);*/
	// temp ^

	return true;
}

void Game::disconnectClient(Client& dc_client) {
	// TODO: Send a PlayerDisconnect packet
	// all_clients => client.reliable_queue.add(playerdisconnect,id);
	OutPacket pdc_packet = OutPacket(PacketType::Reliable, buffer);

	PlayerDisconnect::Fields pdc_fields{ dc_client.id };
	PlayerDisconnect pdc_message = PlayerDisconnect(pdc_fields);

	auto message = std::make_shared<PlayerDisconnect>(pdc_message);
	for (auto& client : connections) {
		if (client.second.get() == &dc_client) continue;
		client.second->reliable_queue.push(message);
	}

	snapshot_manager.removePlayer(dc_client);
}

void Game::receiveMessage(Client& client, InPacket& packet) {
	client.bump(); // Bump the client's last_received timer

	// Receive an unreliable command
	UnreliableCmd command = packet.read<UnreliableCmd>();
	switch (command) {
		case UnreliableCmd::PlayerData: // rename to PlayerData or something like that
			snapshot_manager.updatePlayerState(client, packet); // Update master game state
			break;
		default:
			throw std::invalid_argument("Unknown unreliable command.");
	}

	client.ack(packet.packet_ack);
}

void Game::receiveReliableMessage(Client& client, InPacket& packet) {
	// Receive an unreliable command
	ReliableCmd command = packet.read<ReliableCmd>();
	switch (command) {
		case ReliableCmd::PlayerChat: // rename to PlayerData or something like that
			
			break;
		default:
			throw std::invalid_argument("Unknown reliable command.");
	}

	receiveMessage(client, packet);
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

void Game::broadcastMessage(OutPacket& packet) {
	for (auto conn = connections.begin(); conn != connections.end(); ) { // Loop over clients
		sendMessage(*conn->second, packet);
		++conn;
	}
}

void Game::sendTickMessages() { // TODO: We should consider thread pools!!!!!
	std::vector<std::thread> threads;
	for (auto conn = connections.begin(); conn != connections.end(); ) { // Loop over clients
		if (conn->second->hasTimedOut()) {
			disconnectClient(*conn->second); // Disconnect the client
			conn = connections.erase(conn); // Delete the client instance
		} else {
			threads.emplace_back(
				std::thread(&Game::sendClientTick, this, std::ref(*conn->second.get()))
			);
			++conn;
		}
	}

	for (auto& th : threads) { // Wait for all tick threads to finish
		th.join();
	}
}

void Game::sendClientTick(Client& client) {
	bool send_reliable = client.shouldSendReliable();

	OutPacket tick_packet = OutPacket( // We might not need a packet type at all in the future
		send_reliable ? PacketType::Reliable : PacketType::Unreliable, client.buffer
	);

	// Write optional reliable message
	if (send_reliable) client.appendReliable(tick_packet);

	// Write snapshot message
	tick_packet.write(UnreliableCmd::Snapshot);
	snapshot_manager.writeSnapshot(client, tick_packet);

	sendMessage(client, tick_packet); // Send the packet
}
