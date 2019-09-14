#pragma once

#include "../Utils/CircularBuffer.h"
#include "../Utils/AckBuffer.h"
#include "Snapshot.h"

#include <chrono>


struct SnapshotBuffer {
public:
	SnapshotBuffer(size_t size) : 
		size(size),
		snapshots(new Snapshot*[size]) {}

	void add(Snapshot* snapshot) { // todo: add const
		last_snapshot = snapshot->id % size;
		snapshots[last_snapshot] = snapshot;
	}

	Snapshot& get(unsigned short id) {
		// TODO: Search array for element
	}

	Snapshot& get_last() {
		// TODO: Return last snapshot
	}
private:
	size_t size;
	Snapshot** snapshots;
	unsigned short last_snapshot; // Last snapshot index
};

class Game;

class Client {
public:
	const unsigned char id;
	const unsigned long ip;
	const unsigned short port;

	Game* game;
	unsigned short server_sequence = 1; // Sequences start at 1
	AckBuffer sequences = AckBuffer();
	SnapshotBuffer snapshots = SnapshotBuffer(32);

	// Hold the last 32 sent snapshots. Maybe use an lru/lra cache instead?
	// CircularBuffer snapshots = CircularBuffer<Snapshot>(32);
	//Snapshot last_snapshot_id;

	//Snapshot* snapshots[32];
private:
	std::chrono::steady_clock::time_point last_received;
public:
	Client(Game* client_game, unsigned char id, unsigned long ip, unsigned short port);
	void bump();
};
