#include "Client.h"


Client::Client(Game* client_game, unsigned long ip, unsigned short port) :
	game(client_game),
	ip(ip),
	port(port)
{
	bump();
}

void Client::bump() { // bump time since last packet received (temp)
	last_received = std::chrono::steady_clock::now();
}
