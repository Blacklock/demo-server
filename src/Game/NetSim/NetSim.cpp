#include "NetSim.h"
//#include "../../Socket.h"
#include "../Game.h"

#include <shared_mutex>
#include <iostream>


NetSim::NetSim(Game* game, int) : sim_game(game) {
	int x = 15;
}

void NetSim::startSim() {
	/*for (int i = 1; i <= FAKE_CLIENTS; ++i) {
		InPacketInfo p_info = InPacketInfo();
		p_info.buffer_size = 0;
		p_info.sender_address = i;
		p_info.sender_port = i;
		game->connectClient(i, p_info, true);
	}*/

	/*game->t_pool->push([=](int) {
		this->sendClientTick(conn->second, fake_send);
		return true;
	})*/
}

void NetSim::stopSim() {

}

void NetSim::tickSimulation() {
	
}
