#pragma once

#include "Snapshot.h"


class SnapshotManager {
private:
	Snapshot master_snapshot;
	Snapshot* dummy_snapshot;
public:
	SnapshotManager(Snapshot* dummy_snapshot);
};
