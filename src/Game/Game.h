#pragma once

#include "../Socket.h"
#include "Packet/InPacket.h"
#include "Packet/OutPacket.h"
#include "NetSim/NetSim.h"
#include "Client.h"
#include "Snapshot/SnapshotManager.h"
#include "../Utils/ctpl.h"

#include <boost/pool/object_pool.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>
#include <stdint.h>


class GameServer;

class Game {
public:
	static const unsigned int MAX_PACKET_SIZE = 512;
	typedef boost::object_pool<unsigned char[MAX_PACKET_SIZE]> object_pool;

	Socket* socket;
	object_pool* buf_pool;
	ctpl::thread_pool* t_pool;

	std::unordered_map<long long, std::shared_ptr<Client>> connections;
	std::stack<unsigned char> id_slots; // Available entity IDs
	// Entities waiting for their ID slot to be freed (once all RemoveEntity's have been acked)
	std::unordered_map<unsigned char, unsigned int> dead_entities;
private:
	SnapshotManager snapshot_manager = SnapshotManager();
	std::unique_ptr<NetSim> network_sim;
	uint8_t connections_num = 0; // Amount of connected clients
public:
	Game(Socket* socket, object_pool* buf_pool);
	Game(Socket* socket, object_pool* buf_pool, ctpl::thread_pool* t_pool);

	bool connectClient(long long connection, InPacketInfo p_info, bool simulated_client = false);
	unsigned char createEntity(std::shared_ptr<Entity> entity);
	void removeEntity(unsigned char id);

	void receiveMessage(Client& client, InPacket& packet);
	void receiveReliableMessage(Client& client, InPacket& packet);
	void sendMessage(Client& client, OutPacket& packet, bool fake_send = false);

	void sendTickMessages();
	void sendClientTick(std::shared_ptr<Client> client, bool fake_send);
};
