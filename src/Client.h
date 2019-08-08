#pragma once


enum class ClientStatus { Disconnected, Connecting, Connected };

class Client {
public:
	Client(unsigned long a, unsigned short p);
public:
	unsigned int id;
	ClientStatus status;
	const unsigned long address;
	const unsigned short port;
};
