#include "SnapshotBuffer.h"


SnapshotBuffer::SnapshotBuffer(unsigned short size) :
	size(size),
	snapshots(new Snapshot*[size]()) {}

void SnapshotBuffer::add(Snapshot* snapshot) { // todo: add const
	// Should we delete the previous snapshot that is being replaced?
	// Perhaps use autopointers or whatever
	snapshots[snapshot->id % size] = snapshot;
}

Snapshot* SnapshotBuffer::get(unsigned short id) {
	Snapshot* snapshot = snapshots[id % size];

	if (snapshot != nullptr && snapshot->id == id) return snapshot;

	// This snapshot doesn't exist or is too old; use the master gamestate instead
	return nullptr;
}
