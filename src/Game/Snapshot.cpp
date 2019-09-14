#include "Snapshot.h"

#include <iostream>


Snapshot::Snapshot(unsigned short id, Snapshot* source_snapshot) :
	id(id)
{
	//modified_fields.raw = 0;

	if (!source_snapshot) { // nullptr, it's either a master or dummy snapshot
		
	}
}
