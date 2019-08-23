// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Socket.h"
#include "GameServer.h"

#include <iostream>
#include <string>


int main()
{
    std::cout << "Starting...\n";

	unsigned short serverPort = 12345;

	GameServer server(serverPort);
	//server.createGame();
}
