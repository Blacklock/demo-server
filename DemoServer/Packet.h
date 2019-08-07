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
private:
	unsigned char* buffer = nullptr;
	unsigned short buffer_index = 0;
	// TODO: multiple command/value sets
public:
	Packet(PacketType type, unsigned char buffer_in[]);
	Packet(unsigned char buffer_in[], unsigned short import_size);

	void setPacketLength();

	void write(unsigned char value);
	void write(unsigned short value);
private:
	void build(int buffer_size);
};
