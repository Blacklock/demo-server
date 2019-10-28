#include "Client.h"
#include "../Game.h" // ...works?


Client::Client(Game* client_game, unsigned char id, unsigned long ip, unsigned short port) :
	game(client_game),
	id(id),
	ip(ip),
	port(port),
	snapshots(SnapshotBuffer(32))
{
	bump();
}

void Client::send(OutPacket& packet) {
	packet.setPacketLength();
	game->socket->sendPacket(packet.buffer, packet.packet_length, ip, port);
}

void Client::bump() { // bump time since last packet received (temp)
	last_received = std::chrono::steady_clock::now();
}

bool Client::hasTimedOut() {
	if (!TIMEOUT_ENABLED) return false; // c++17: if constexpr

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	long long ms_since = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_received).count();

	return ms_since > TIMEOUT_MS;
}
