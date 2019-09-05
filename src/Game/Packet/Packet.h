#pragma once


enum class PacketType : unsigned char {
	Unreliable = 'U', // Fire-and-forget commands
	Reliable = 'R', // Reliable commands, ensured to arrive
	Control = 'C', // Reliable commands, used for connection
	Fragment = 'F' // Packet fragment
};

enum class UnreliableCmd : unsigned char {
	PlayerSnapshot,
	Snapshot
};

enum class ReliableCmd : unsigned char {
	PlayerJoin,
	PlayerChange,
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
	unsigned char* buffer = nullptr;
protected:
	unsigned short buffer_index = 0;
};
