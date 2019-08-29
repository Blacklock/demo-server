// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GameServer.h"

#include <iostream>


int main()
{
    std::cout << "Starting...\n";

	unsigned short serverPort = 12345;

	GameServer server(serverPort);
	//server.createGame();
}
