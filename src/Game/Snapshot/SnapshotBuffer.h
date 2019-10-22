#pragma once

#include "Snapshot.h"

#include <memory>
#include <vector>


class SnapshotBuffer {
public:
	unsigned short last_snapshot; // We may not need this member, use client.last_snapshot instead?
private:
	unsigned short size;
	std::vector<std::shared_ptr<Snapshot>> snapshots;
public:
	SnapshotBuffer(unsigned short size);

	void add(std::shared_ptr<Snapshot> snapshot);
	std::shared_ptr<Snapshot> get(unsigned short id);
};
