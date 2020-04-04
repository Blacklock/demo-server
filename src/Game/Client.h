#pragma once

#include "Packet/OutPacket.h"
#include "../Utils/AckBuffer.h"
#include "../Utils/CircularBuffer.h"
#include "Snapshot/SnapshotBuffer.h"
#include "Message/ReliableMessage.h"
#include "../Config.h"

#include <chrono>
#include <queue>
#include <set>
#include <memory>
#include <iostream>


struct TimestampedId {
	int id;
	std::chrono::steady_clock::time_point timestamp;

	TimestampedId() : id(0), timestamp(std::chrono::steady_clock::now()) {}

	TimestampedId(int id, std::chrono::steady_clock::time_point timestamp) : id(id), timestamp(timestamp) {}

	void operator=(const int value) {
		if (id != -1 && value == -1) { // has been acked
			//auto time_now = std::chrono::steady_clock::now();
			//std::cout << "Ping: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - timestamp).count() << "ms\n";
		}
		id = value;
	}

	bool operator==(const int value) {
		return id == value;
	}
};

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
	
	CircularBuffer<bool> packet_loss_tracker = CircularBuffer<bool>(config::PACKET_LOSS_COUNT);
	CircularBuffer<TimestampedId> unacked_ids = CircularBuffer<TimestampedId>(33);
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
