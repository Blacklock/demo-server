#pragma once


enum class PacketType : unsigned char { 
	Unreliable = 'U', // Fire-and-forget commands
	Reliable = 'R', // Reliable commands, ensured to arrive
	Control = 'C' // Reliable commands, used for connection 
};

enum class UnreliableCmd : unsigned char {};

enum class ReliableCmd : unsigned char {};

enum class ControlCmd : unsigned char {
	ConnRequest = 'R', // Client requesting connection [control command, protocol]
	ConnAccept = 'A', // Server accepting the client's connection request [control command]
	ConnDeny = 'D' // Server denying the client's connection request [control command]
};

union Command {
	UnreliableCmd u_cmd;
	ReliableCmd r_cmd;
	ControlCmd c_cmd;
};

class Packet {
public:
	PacketType packet_type;
	unsigned short packet_length;
	Command packet_cmd;
	// TODO: receiving address/port

	Packet(unsigned char buffer[], int buffer_size);
};
