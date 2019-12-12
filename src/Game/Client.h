#pragma once

#include "Packet/OutPacket.h"
#include "../Utils/AckBuffer.h"
#include "../Utils/CircularBuffer.h"
#include "Snapshot/SnapshotBuffer.h"
#include "Message/ReliableMessage.h"

#include <chrono>
#include <queue>


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

	AckBuffer sequences = AckBuffer();
	unsigned short server_sequence = 1; // Sequences start at 1
	unsigned short last_snapshot = 0; // Last acked snapshot's ID
private:

	std::chrono::steady_clock::time_point last_received;
	std::chrono::steady_clock::time_point last_reliable_sent;
	
	// Wait for any of the packets where we sent the reliable message to get acked
	CircularBuffer<unsigned short> reliable_ids = CircularBuffer<unsigned short>(8);
public:
	Client(Game* client_game, unsigned char id, unsigned long ip, unsigned short port);

	void send(OutPacket& packet); // we should make unreliable packets Message classes too!
	void ack(InPacket& packet);
	bool shouldSendReliable();
	void appendReliable(OutPacket& packet);
	void bump();
	bool hasTimedOut();
};
