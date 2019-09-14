#include "SnapshotBuffer.h"


SnapshotBuffer::SnapshotBuffer(unsigned short size) :
	size(size),
	snapshots(new Snapshot*[size]()) {}

void SnapshotBuffer::add(Snapshot* snapshot) { // todo: add const
	last_snapshot = snapshot->id % size;
	snapshots[last_snapshot] = snapshot;
}

Snapshot* SnapshotBuffer::get(unsigned short id) {
	return snapshots[id % size];
}

Snapshot* SnapshotBuffer::get_last() {
	return snapshots[last_snapshot];
}
