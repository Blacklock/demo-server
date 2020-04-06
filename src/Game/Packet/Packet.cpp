#include "Packet.h"


unsigned short Packet::getBufferIndex() {
	return buffer_index;
}

void Packet::setBufferIndex(unsigned short new_index) {
	buffer_index = new_index;
}
