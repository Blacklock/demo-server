#include "Snapshot.h"

#include <iostream>


Snapshot::Snapshot(Snapshot* source_snapshot) {
	modified_fields.raw = 0;

	if (!source_snapshot) { // nullptr, it's either a master or dummy snapshot
		// nothing yet
	}
}

void Snapshot::read(InPacket packet) { // Reads a PlayerSnapshot
	unsigned char field_flags = packet.read<unsigned char>();

	// Iterate over field flags
	for (int i = 0; i != (int)SnapshotFields::End; i++) {
		std::cout << i << "\n";

		if (field_flags & 1) { // Field has been changed
			switch ((SnapshotFields)i) {
				case SnapshotFields::PosX:
					//pos_x = packet.read<int32_t>();
					break;
				case SnapshotFields::PosY:
					//pos_y = packet.read<int32_t>();
					break;
				case SnapshotFields::Score:
					//score = packet.read<unsigned char>();
					break;
				default:
					throw std::invalid_argument("Unknown snapshot field.");
			}
		}

		field_flags >>= 1;
	}
}
