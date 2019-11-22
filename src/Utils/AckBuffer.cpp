#include "AckBuffer.h"

#include <iostream>
#include <bitset>


AckBuffer::AckBuffer() {}

// Returns true if new sequence is larger than previous
bool AckBuffer::put(unsigned short sequence) {
	if (empty) {
		last_sequence = sequence;
		empty = false;
		return true;
	}

	int difference;
	bool larger = false;
	bool overflow = false;

	// Check if new sequence is larger than previous

	if ((sequence > last_sequence) && (sequence - last_sequence <= 32678)) {
		larger = true;
		difference = sequence - last_sequence;
	} else if ((last_sequence > sequence) && (last_sequence - sequence > 32768)) {
		larger = true;
		overflow = true;
		difference = sequence - last_sequence + 65536; // Overflow
	} else {
		// Insert sequence into bitfield
		difference = overflow ? abs(sequence - last_sequence - 65536) : last_sequence - sequence;

		if (difference > 0 && difference <= 32) {
			ack_bitfield = ack_bitfield | (uint32_t)pow(2, difference - 1);
		}

		return false; // Sequence is smaller than latest
	}

	ack_bitfield = ack_bitfield << difference;
	ack_bitfield = ack_bitfield + (uint32_t)pow(2, difference - 1);

	//std::cout << "seq " << sequence << " last_seq " << last_sequence << " diff " << difference << " <> " << std::bitset<32>(ack_bitfield).to_string() << "\n\n";

	last_sequence = sequence;
	return true;
}

void AckBuffer::reset() {
	empty = true;
	ack_bitfield = 0;
}
