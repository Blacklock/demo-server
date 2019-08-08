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
protected:
	unsigned char* buffer = nullptr;
	unsigned short buffer_index = 0;
};

class InPacket : public Packet {
public:
	InPacket(unsigned char buffer_in[], unsigned short import_size);

	template<typename T> T read();
private:
	void build(int buffer_size);
};

class OutPacket : public Packet {
public:
	OutPacket(PacketType type, unsigned char buffer_in[]);

	void setPacketLength();

	void write(ControlCmd value);
	void write(unsigned char value);
	void write(unsigned short value);
};
