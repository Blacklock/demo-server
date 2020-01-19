#pragma once


enum class PacketType : unsigned char {
	Unreliable, // Fire-and-forget commands
	Reliable, // Reliable commands, ensured to arrive
	Control, // Reliable commands, used for connection
	Fragment // Packet fragment
};

// c++ bitfield order depends on the compiler, so we got rid of it and are just using bitwise operators now
// f = reliable flip
// id0 f0, <ACK id0>, id2 f0, <ACK id2 WITHOUT EXECUTIING, CAUSE f0 RECEIVED>, 
// id3 f0, <ACK id3>, id4 f1, <ACK id4>, id1 f0, <CORRECT f0 RECEIVED, BUT ID SMALLER THAN LAST ACK, NO EXEC ONLY ACK>

enum class UnreliableCmd : unsigned char {
	Ping, // Client sending an empty unreliable packet to prevent a timeout
	PlayerData, // Client sending its position etc
	Snapshot // Server sending a snapshot with game data
};

enum class ReliableCmd : unsigned char {
	RemoveEntity, // Server announcing that an entity has been removed
	PlayerChat // Server announcing a player chat message
};

enum class ControlCmd : unsigned char {
	ConnRequest = 'R', // Client requesting connection [control command, protocol]
	ConnAccept = 'A', // Server accepting the client's connection request [control command]
	ConnDeny = 'D' // Server denying the client's connection request [control command]
};

class Packet {
public:
	PacketType packet_type;
	bool reliable_switch;
	unsigned short packet_sequence;

	unsigned char* buffer = nullptr;
protected:
	unsigned short buffer_index = 0;
};
