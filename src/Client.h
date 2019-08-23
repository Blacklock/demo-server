#pragma once


class Game;

class Client {
public:
	Game* game;

	double pos_x;
	double pos_y;
public:
	Client(Game* client_game);
};
