#include "Game.h"
#include "Game/Packet/Packet.h"
#include "GameServer.h"
#include "Game/Snapshot/Snapshot.h"
#include "Game/Message/PlayerDisconnect.h"
#include "Config.h"

#include <iostream>
#include <stdint.h>


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
			// Update master game state
			snapshot_manager.updatePlayerState(client, packet);
			break;
		default:
			throw std::invalid_argument("Unknown command.");
	}

	client.ack(packet.packet_ack);
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

void Game::sendTickMessages() {
	for (auto conn = connections.begin(); conn != connections.end(); ) { // Loop over clients
		if (conn->second->hasTimedOut()) {
			disconnectClient(*conn->second); // Disconnect the client
			conn = connections.erase(conn); // Delete the client instance
		} else {
			bool send_reliable = conn->second->shouldSendReliable();

			OutPacket tick_packet = OutPacket( // We might not need a packet type at all in the future
				send_reliable ? PacketType::Reliable : PacketType::Unreliable, buffer
			);

			// Write optional reliable message
			if (send_reliable) conn->second->appendReliable(tick_packet);

			// Write snapshot message
			tick_packet.write(UnreliableCmd::Snapshot);
			snapshot_manager.writeSnapshot(*conn->second, tick_packet);

			sendMessage(*conn->second, tick_packet); // Send the packet
			++conn;
		}
	}
}

