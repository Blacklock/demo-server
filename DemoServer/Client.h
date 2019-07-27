#pragma once


enum class ClientStatus { Disconnected, Connecting, Connected };

class Client {
private:
	unsigned int id;
	ClientStatus status;
	unsigned int address;
	unsigned int port;
};
