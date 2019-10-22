#include "SnapshotBuffer.h"


SnapshotBuffer::SnapshotBuffer(unsigned short size) :
	size(size),
	snapshots(size) {}

void SnapshotBuffer::add(std::shared_ptr<Snapshot> snapshot) { // todo: add const
	snapshots[snapshot->id % size] = snapshot;
}

std::shared_ptr<Snapshot> SnapshotBuffer::get(unsigned short id) {
	std::shared_ptr<Snapshot> snapshot = snapshots[id % size];
	if (snapshot != nullptr && snapshot->id == id) return snapshot;

	// This snapshot doesn't exist or is too old; use the master gamestate instead
	return nullptr;
}
