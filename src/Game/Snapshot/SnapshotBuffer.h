#pragma once

#include "Snapshot.h"

#include <memory>
#include <vector>


class SnapshotBuffer {
private:
	unsigned short size;
	std::vector<std::shared_ptr<Snapshot>> snapshots;
	unsigned short last_snapshot; // this could be a pointer
public:
	SnapshotBuffer(unsigned short size);
	void add(std::shared_ptr<Snapshot> snapshot);
	std::shared_ptr<Snapshot> get(unsigned short id);
};
