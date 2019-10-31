// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GameServer.h"
#include "Config.h"

#include <iostream>


int main()
{
	config::load_config("settings.xml");
	unsigned short serverPort = 12345;
	GameServer server(serverPort);
}
