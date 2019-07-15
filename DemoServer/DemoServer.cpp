// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "PacketManager.h"

#include <iostream>
#include <string>


int main()
{
    std::cout << "Starting...\n";
	try {
		PacketManager server = PacketManager(25445);

		//char destIp[INET_ADDRSTRLEN] = "127.0.0.1";
		//server.sendPacket("Hello, world!", "127.0.0.1", 25444);

		while (true) {
			std::cout << "\n\n\nWould you like to send (1) or receive (2) packets?\n";
			
			std::string input;
			getline(std::cin, input);

			if (input == "1" || input == "s") {
				std::cout << "Packet data: ";

				std::string input2;
				getline(std::cin, input2);

				char packetData[1024];
				strcpy_s(packetData, input2.c_str());

				server.sendPacket(packetData, "127.0.0.1", 25445);
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
	}
}
