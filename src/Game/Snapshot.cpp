#include "Snapshot.h"

#include <iostream>


Snapshot::Snapshot(Snapshot* source_snapshot) {
	//modified_fields.raw = 0;

	if (!source_snapshot) { // nullptr, it's either a master or dummy snapshot
		
	}
}
