#include "Client.h"


Client::Client(Game* client_game) {
	game = client_game;
	bump();
}

void Client::bump() { // bump time since last packet received (temp)
	last_received = std::chrono::steady_clock::now();
}
