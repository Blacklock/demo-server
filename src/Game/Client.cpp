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
	if (packet.packet_type == PacketType::Reliable) {
		reliable_ids.put(packet.packet_sequence);
		last_reliable_sent = std::chrono::steady_clock::now();
	}

	packet.setPacketLength();
	game->socket->sendPacket(packet.buffer, packet.packet_length, ip, port);
}

void Client::ack(unsigned short packet_id) {
	sequences.put(packet_id); // Update ack

	// If newly received ack is larger than previous, update last received snapshot
	last_snapshot = sequences.last_sequence;

	if (reliable_ids.find(packet_id)) { // Reliable message acked
		reliable_ids.reset();
		last_reliable_sent = std::chrono::steady_clock::now();
		reliable_queue.pop();
	}
}

bool Client::shouldSendReliable() {
	if (reliable_queue.empty()) return false;

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_reliable_sent).count();

	return ms > config::RELIABLE_RESEND_MS;
}

void Client::appendReliable(OutPacket& packet) {
	if (reliable_queue.empty()) throw std::exception("Cannot append reliable message, queue is empty.");
	reliable_queue.front()->serialize(packet);
}

void Client::bump() {
	last_received = std::chrono::steady_clock::now();
}

bool Client::hasTimedOut() {
	if (config::TIMEOUT_MS <= 0) return false;

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_received).count();

	return ms > config::TIMEOUT_MS;
}
