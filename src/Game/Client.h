#pragma once

#include "Packet/OutPacket.h"
#include "../Utils/AckBuffer.h"
#include "../Utils/CircularBuffer.h"
#include "Snapshot/SnapshotBuffer.h"
#include "Message/ReliableMessage.h"

#include <chrono>
#include <queue>
#include <set>
#include <memory>


class Game;

class Client {
public:
	const unsigned char id;
	const unsigned long ip;
	const unsigned short port;

	Game* game;
	SnapshotBuffer snapshots;
	std::queue<std::shared_ptr<ReliableMessage>> reliable_queue;

	unsigned char buffer[512]; // MAX_PACKET_SIZE (!)

	AckBuffer sequences = AckBuffer(); // Sequences we've received from the client
	unsigned short server_sequence = 1; // Sequences start at 1
	unsigned short last_snapshot = 0; // Last snapshot acked by the client

	// Flip for every new reliable message
	bool server_rel_switch = 0;
	bool client_rel_switch = 0;
private:
	std::chrono::steady_clock::time_point last_received;
	std::chrono::steady_clock::time_point last_reliable_sent;
	bool send_reliable_instantly = false;
	
	CircularBuffer<int> unacked_ids = CircularBuffer<int>(32);
	std::set<unsigned short> reliable_ids = std::set<unsigned short>();
public:
	Client(Game* client_game, unsigned char id, unsigned long ip, unsigned short port);
	~Client();

	// TODO: we should make unreliable packets Message classes too!
	void send(OutPacket& packet, bool fake_send = false);
	void ack(InPacket& packet);
	bool shouldSendReliable();
	void appendReliable(OutPacket& packet);
	void bump();
	bool hasTimedOut();
private:
	void nextReliable();
};
