#pragma once

#include "Snapshot.h"


class SnapshotBuffer {
private:
	unsigned short size;
	Snapshot** snapshots;
	unsigned short last_snapshot; // this could be a pointer
public:
	SnapshotBuffer(unsigned short size);
	void add(Snapshot* snapshot);
	Snapshot* get(unsigned short id);
};
