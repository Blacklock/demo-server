#pragma once

#include "Snapshot.h"


class SnapshotManager {
private:
	static Snapshot dummy_snapshot;
	Snapshot master_snapshot = Snapshot(nullptr);
public:
	
};
