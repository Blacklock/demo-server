#pragma once


enum class PacketType : unsigned char {
	Unreliable, // Fire-and-forget commands
	Reliable, // Reliable commands, ensured to arrive
	Control, // Reliable commands, used for connection
	Fragment // Packet fragment
};

union BasePacketHeader { // Shows which fields have been modified
	struct {
		PacketType packet_type : 2;
		bool reliable_switch : 1; // Reliable sequence switch (prevent resending of messages)
		bool empty : 5;
	} fields;
	unsigned char raw;
};

// f = reliable flip
// id0 f0, <ACK id0>, id2 f0, <ACK id2 WITHOUT EXECUTIING, CAUSE f0 RECEIVED>, 
// id3 f0, <ACK id3>, id4 f1, <ACK id4>, id1 f0, <CORRECT f0 RECEIVED, BUT ID SMALLER THAN LAST ACK, NO EXEC ONLY ACK>

enum class UnreliableCmd : unsigned char {
	Ping, // Client sending an empty unreliable packet to prevent a timeout
	PlayerData, // Client sending its position etc
	Snapshot // Server sending a snapshot with game data
};

enum class ReliableCmd : unsigned char {
	PlayerDisconnect, // Server announcing that a player has disconnected
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
	unsigned short packet_length;
	unsigned short packet_sequence;

	unsigned char* buffer = nullptr;
protected:
	unsigned short buffer_index = 0;
	// unsigned short real_buffer_index = 0; // saves the end of buffer, for later
};
