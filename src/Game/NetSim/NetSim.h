#pragma once

//#include "../Game/Game.h"
//#include "../../Utils/ctpl.h"
//#include "../../Config.h"


class Game;

class NetSim {
public:
	const int FAKE_CLIENTS = 5;
private:
	Game* sim_game;
public:
	NetSim(Game* game, int);

	void startSim();
	void stopSim();
private:
	void tickSimulation();
};

