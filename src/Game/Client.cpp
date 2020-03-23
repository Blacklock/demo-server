#include "Client.h"
#include "Game.h"

#include <plog/Log.h>
#include <stdint.h>
#include <iostream>


Client::Client(Game* client_game, unsigned char id, unsigned long ip, unsigned short port) :
	game(client_game),
	id(id),
	ip(ip),
	port(port),
	snapshots(SnapshotBuffer(32))
{
	bump();
}

Client::~Client() {
	while (!reliable_queue.empty()) {
		reliable_queue.front()->onFail(*this);
		reliable_queue.pop();
	}
}

void Client::send(OutPacket& packet, bool fake_send) {
	if (packet.packet_type == PacketType::Reliable) {
		reliable_ids.emplace(packet.packet_sequence);
		last_reliable_sent = std::chrono::steady_clock::now();
		send_reliable_instantly = false;
	}

	if (unacked_ids.isFull()) {
		packet_loss_tracker.put(unacked_ids.getLast() == -1);
		double ratio = (double)packet_loss_tracker.count(true) / (double)packet_loss_tracker.getSize();
		LOGV << ratio << "\n";
	}

	unacked_ids.put(packet.packet_sequence);

	if (!fake_send) game->socket->sendPacket(packet.buffer, packet.getBufferIndex(), ip, port);
}

void Client::ack(InPacket& packet) {
	//if (reliable_ids.getSize() == 0) return; // No reliable message

	// Find out if our reliable message has been acked
	if (unacked_ids.replace(static_cast<int>(packet.packet_ack), -1)) {
		if (reliable_ids.find(packet.packet_ack) != reliable_ids.end()) {
			nextReliable();
		}
	}

	uint32_t bitfield = packet.ack_bitfield;

	for (int i = 1; bitfield > 0; ++i, bitfield >>= 1) {
		int bitfield_ack = (packet.packet_ack - i + 65536) % 65536;
		if ((bitfield & 1) == 1 && unacked_ids.replace(bitfield_ack, -1)) {
			if (reliable_ids.find(static_cast<unsigned short>(bitfield_ack)) != reliable_ids.end()) {
				nextReliable();
			}
		}
	}
}

bool Client::shouldSendReliable() {
	if (reliable_queue.empty()) return false;
	if (send_reliable_instantly) return true;

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

void Client::nextReliable() { // Mark reliable message as received
	reliable_ids.clear();
	reliable_queue.front()->onAck(*this);
	reliable_queue.pop();
	send_reliable_instantly = true; // Send next reliable message instantly after this one
	server_rel_switch = !server_rel_switch; // Flip the reliable sequence
}
