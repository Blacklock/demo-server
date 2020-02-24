#include "Game.h"
#include "Packet/Packet.h"
#include "../GameServer.h"
#include "Snapshot/Snapshot.h"
#include "Message/RemoveEntity.h"
#include "Message/PlayerChat.h"
#include "Entity/Player.h"
#include "Entity/Server.h"
#include "../Config.h"

#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <stdint.h>


Game::Game(Socket* socket) : socket(socket) {
	for (int i = 255; i >= 0; --i) id_slots.push(static_cast<unsigned char>(i));

	Server::State server_state = { /*.status=*/ 20 };
	unsigned char server_id = createEntity(std::make_shared<Server>(server_state));
	assert(server_id == 0);
}

bool Game::connectClient(long long connection, InPacketInfo p_info) {
	if (connections_num >= config::MAX_CONNECTIONS) return false;

	unsigned char client_id = createEntity(std::make_shared<Player>());

	connections[connection] = std::make_unique<Client>(
		this, client_id, p_info.sender_address, p_info.sender_port
	);

	std::string join_msg = "Player " + std::to_string(client_id) + " has joined the game.";
	PlayerChat::Fields pc_fields{ 0, static_cast<uint8_t>(join_msg.length()), join_msg };
	auto message = std::make_shared<PlayerChat>(pc_fields);
	for (auto& bc_client : connections) bc_client.second->reliable_queue.push(message);

	std::cout << "Accepted connection " << static_cast<int>(client_id) << "\n";
	connections_num++;
	return true;
}

unsigned char Game::createEntity(std::shared_ptr<Entity> entity) {
	unsigned char id = id_slots.top();
	id_slots.pop();

	snapshot_manager.master_snapshot.entities[id] = entity;

	return id;
}

void Game::removeEntity(unsigned char id) {
	RemoveEntity::Fields re_fields{ id };
	auto message = std::make_shared<RemoveEntity>(re_fields);

	dead_entities[id] = 0;

	for (auto& bc_client : connections) { // Broadcast the RemoveEntity message
		if (bc_client.second->id == id) continue;
		dead_entities[id]++;
		bc_client.second->reliable_queue.push(message);
	}

	if (dead_entities[id] == 0) { // Manually delete
		dead_entities.erase(id);
		id_slots.push(id);
	}

	// Remove the entity from the master snapshot
	auto player_state = snapshot_manager.master_snapshot.entities.find(id);
	if (player_state != snapshot_manager.master_snapshot.entities.end()) {
		snapshot_manager.master_snapshot.entities.erase(player_state->first);
	}
}

void Game::receiveMessage(Client& client, InPacket& packet) {
	client.bump(); // Bump the client's last_received timer

	bool newer = client.sequences.put(packet.packet_sequence); // Update our ack bitfield

	// Receive an unreliable command
	UnreliableCmd command = packet.read<UnreliableCmd>();
	switch (command) { // A GameLogic class or something eventually, perhaps?
		case UnreliableCmd::Ping:
			break;
		case UnreliableCmd::PlayerData:
			if (newer) {
				std::shared_ptr<Player> player = snapshot_manager.getEntity<Player>(client.id);
				player->read(packet);
			}
			break;
		default:
			throw std::invalid_argument("Unknown unreliable command.");
	}

	if (newer) client.last_snapshot = packet.packet_ack;
	client.ack(packet);
}

void Game::receiveReliableMessage(Client& client, InPacket& packet) {
	// Check if the received reliable message is a resend of previous
	bool unique_message = false;
	if (packet.reliable_switch == client.client_rel_switch) {
		unique_message = true;
		client.client_rel_switch = !client.client_rel_switch;
	}

	// Receive an unreliable command
	ReliableCmd command = packet.read<ReliableCmd>();
	switch (command) {
		case ReliableCmd::PlayerChat:
		{
			PlayerChat chat_message = PlayerChat(packet);
			if (unique_message) {
				chat_message.fields.entity_id = client.id;
				auto message = std::make_shared<PlayerChat>(chat_message);
				for (auto& i_client : connections) {
					i_client.second->reliable_queue.push(message);
				}
			}
			break;
		}
		default:
			throw std::invalid_argument("Unknown reliable command.");
	}

	receiveMessage(client, packet);
}

void Game::sendMessage(Client& client, OutPacket& packet, bool fake_send) {
	packet.setHeaders(
		client.server_sequence, 
		client.sequences.empty ? (unsigned short)0 : client.sequences.last_sequence,
		client.sequences.ack_bitfield
	);

	client.server_sequence++;

	client.send(packet, fake_send);
}

void Game::sendTickMessages() { // TODO: We should consider thread pools!!!!!
	std::vector<std::thread> threads;
	for (auto conn = connections.begin(); conn != connections.end(); ) { // Loop over clients
		if (conn->second->hasTimedOut()) {
			std::cout << "Connection " << static_cast<int>(conn->second->id) << " has timed out.\n";
			std::string dc_msg = "Player " + std::to_string(conn->second->id) + " has disconnected.";

			removeEntity(conn->second->id); // Remove the player entity
			conn = connections.erase(conn); // Delete the client instance

			PlayerChat::Fields pc_fields{ 0, static_cast<uint8_t>(dc_msg.length()), dc_msg };
			auto message = std::make_shared<PlayerChat>(pc_fields);
			for (auto& bc_client : connections) bc_client.second->reliable_queue.push(message);
			connections_num--;
		} else {
			// Simulated outgoing packet loss rate
			float r = (float)rand() / RAND_MAX;
			if (r < config::OUT_LOSS) {
				std::cout << "out fail " << r << "\n";
			}
			threads.emplace_back(
				std::thread(&Game::sendClientTick, this, std::ref(*conn->second.get()), r < config::OUT_LOSS)
			);
		
			++conn;
		}
	}

	for (auto& th : threads) { // Wait for all tick threads to finish
		th.join();
	}
}

void Game::sendClientTick(Client& client, bool fake_send) {
	bool send_reliable = client.shouldSendReliable();

	OutPacket tick_packet = OutPacket( // We might not need a packet type at all in the future
		send_reliable ? PacketType::Reliable : PacketType::Unreliable, 
		client.buffer,
		send_reliable ? client.server_rel_switch : false
	);

	// Write optional reliable message
	if (send_reliable) client.appendReliable(tick_packet);

	// Write snapshot message
	tick_packet.write(UnreliableCmd::Snapshot);

	// Make sure that the packet isn't empty before sending it
	if (snapshot_manager.writeSnapshot(client, tick_packet) || send_reliable) {
		sendMessage(client, tick_packet, fake_send);
	}
}
