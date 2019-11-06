#include "Entity.h"

#include "../../Config.h"


bool Entity::writeDeltaField(OutPacket& packet, uint8_t new_field, uint8_t old_field) {
	if (new_field != old_field) {
		if (config::DEBUG) std::cout << static_cast<int>(new_field) << "\n";
		packet.write(new_field);
		return true;
	}
	if (config::DEBUG) std::cout << "Unchanged\n";
	return false;
}

bool Entity::writeDeltaField(OutPacket& packet, int32_t new_field, int32_t old_field, bool encode) {
	if (new_field != old_field) {
		if (config::DEBUG) std::cout << new_field << " (old field: " << old_field << ")\n";

		if (encode) { // (S)LEB128, vbyte encoding
			bool more = true;
			int sign = new_field >> 31; // 0 = unsigned; -1 = signed

			do {
				uint8_t byte = new_field & 0b01111111; // get the 7 least significant bits
				new_field >>= 7; // assumes arithmetic shift

				if (new_field == sign && ((byte ^ sign) & 0b01000000) == 0) {
					more = false;
					byte |= 0b10000000; // set the most significant bit to mark the end
				}

				packet.write(byte);
			} while (more);
		} else {
			packet.write(new_field);
		}

		return true;
	}
	if (config::DEBUG) std::cout << "Unchanged\n";
	return false;
}
