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
	unsigned short sendPort = 23456;

	/*std::cout << "Set your receiving port number: ";
	std::cin >> serverPort;
	std::cout << "Set your sending port number: ";
	std::cin >> sendPort;
	std::cin.ignore(1, '\n');*/

	//GameServer server = GameServer(serverPort);
	GameServer server(serverPort);

	/*std::string input2;
	getline(std::cin, input2);
	char packetData[1024];
	strcpy_s(packetData, input2.c_str());

	server.send(packetData, "127.0.0.1", sendPort);*/

	/*while (true) {
		std::string input;
		getline(std::cin, input);

		char packetData[1024];
		strcpy_s(packetData, input.c_str());

		//server.sendPacket(packetData, "127.0.0.1", sendPort);
	}*/




	/*try {
		unsigned short serverPort;
		unsigned short sendPort;

		std::cout << "Set your receiving port number: ";
		std::cin >> serverPort;
		std::cout << "Set your sending port number: ";
		std::cin >> sendPort;

		Socket server;
		server.create(serverPort);

		std::cin.ignore(1, '\n');

		while (true) {
			std::cout << "\n\nWould you like to send (1) or receive (2) packets?\n";
			
			std::string input;
			getline(std::cin, input);

			if (input == "1" || input == "s") {
				std::cout << "Packet data: ";

				std::string input2;
				getline(std::cin, input2);

				char packetData[1024];
				strcpy_s(packetData, input2.c_str());

				server.sendPacket(packetData, "127.0.0.1", sendPort);
			}
			else if (input == "2" || input == "r") {
				server.receivePackets();
			}
			else {
				std::cout << "Invalid input. ";
			}
		}
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what();
	}*/
}
