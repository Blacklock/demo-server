#include "Client.h"
#include "../Game.h" // ...works?

class Game; // do we need this?

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

	//socket.sendPacket(buffer, packet.packet_length, ip, port);
}

void Client::bump() { // bump time since last packet received (temp)
	last_received = std::chrono::steady_clock::now();
}
