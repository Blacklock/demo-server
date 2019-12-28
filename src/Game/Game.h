#pragma once

#include "../Socket.h"
#include "Packet/InPacket.h"
#include "Packet/OutPacket.h"
#include "Client.h"
#include "Snapshot/SnapshotManager.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>
#include <stdint.h>


class GameServer;

class Game {
public:
	Socket* socket;
	std::unordered_map<long long, std::unique_ptr<Client>> connections;

	std::stack<unsigned char> id_slots; // Available entity IDs
	// Entities waiting for their ID slot to be freed (once all RemoveEntity's have been acked)
	std::unordered_map<unsigned char, unsigned int> dead_entities;
private:
	static const unsigned int MAX_PACKET_SIZE = 512;
	unsigned char buffer[MAX_PACKET_SIZE];

	SnapshotManager snapshot_manager = SnapshotManager();
	uint8_t connections_num = 0; // Amount of connected clients
public:
	Game(Socket* socket);

	bool connectClient(long long connection, InPacketInfo p_info);
	unsigned char createEntity(std::shared_ptr<Entity> entity);
	void removeEntity(unsigned char id);

	void receiveMessage(Client& client, InPacket& packet);
	void receiveReliableMessage(Client& client, InPacket& packet);
	void sendMessage(Client& client, OutPacket& packet);

	void sendTickMessages();
	void sendClientTick(Client& client);
};
