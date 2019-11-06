#pragma once

#include "Snapshot.h"

#include <memory>
#include <vector>


class SnapshotBuffer {
private:
	const unsigned short size;
	std::vector<std::shared_ptr<Snapshot>> snapshots;
public:
	SnapshotBuffer(unsigned short size);

	void add(std::shared_ptr<Snapshot> snapshot); // pass raw?
	std::shared_ptr<Snapshot> get(unsigned short id);
};
