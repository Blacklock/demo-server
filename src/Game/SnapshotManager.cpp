#include "SnapshotManager.h"


SnapshotManager::SnapshotManager(Snapshot* dummy_snapshot) :
	master_snapshot(nullptr),
	dummy_snapshot(dummy_snapshot) {}
