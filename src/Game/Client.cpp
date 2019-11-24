#include "Client.h"
#include "../Game.h" // ...works?
#include "../Config.h"


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

void Client::ack(unsigned short id) {
	sequences.put(id); // Update ack

	// If newly received ack is larger than previous, update last received snapshot
	last_snapshot = sequences.last_sequence;

	if (reliable_ids.find(id)) { // Reliable message acked
		reliable_ids.reset();
		reliable_queue.pop();
	}
}

/*ReliableMessage* Client::getReliable() {
	return reliable_queue.empty() ? nullptr : reliable_queue.front().get();
}*/

bool Client::reliableQueueEmpty() {
	return reliable_queue.empty();
}

void Client::addReliable(ReliableMessage* message) {
	reliable_queue.push(std::make_shared<ReliableMessage>(message));
}

void Client::appendReliable(OutPacket& packet) {
	if (reliableQueueEmpty()) throw std::exception("Cannot append reliable message, queue is empty.");
	reliable_queue.front()->serialize(packet);
}

void Client::bump() {
	last_received = std::chrono::steady_clock::now();
}

bool Client::hasTimedOut() {
	if (config::TIMEOUT_MS <= 0) return false;

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	long long ms_since = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_received).count();

	return ms_since > config::TIMEOUT_MS;
}
