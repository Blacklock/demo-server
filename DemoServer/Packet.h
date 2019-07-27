#pragma once


enum class PacketType : unsigned char { 
	Unreliable = 'U', // Fire-and-forget commands
	Reliable = 'R', // Reliable commands, ensured to arrive
	Control = 'C' // Reliable commands, used for connection 
};

class Packet {
public:
	PacketType packet_type;
	unsigned short packet_length;

	Packet(unsigned char data[], int buffer_size);
};
