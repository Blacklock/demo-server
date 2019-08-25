#pragma once


enum class PacketType : unsigned char {
	Unreliable = 'U', // Fire-and-forget commands
	Reliable = 'R', // Reliable commands, ensured to arrive
	Control = 'C' // Reliable commands, used for connection 
};

enum class UnreliableCmd : unsigned char {
	PosUpdate
};

enum class ReliableCmd : unsigned char {
	PlayerJoin,
	PlayerLeave
};

enum class ControlCmd : unsigned char {
	ConnRequest = 'R', // Client requesting connection [control command, protocol]
	ConnAccept = 'A', // Server accepting the client's connection request [control command]
	ConnDeny = 'D' // Server denying the client's connection request [control command]
};

class Packet {
public:
	PacketType packet_type;
	unsigned short packet_length;
	unsigned short packet_sequence;
protected:
	unsigned char* buffer = nullptr;
	unsigned short buffer_index = 0;
};
