// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

// Platform detection

#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC 2
#define PLATFORM_UNIX 3

#ifdef _WIN32
	#define PLATFORM PLATFORM_WINDOWS
#elif __APPLE__
	#define PLATFORM PLATFORM_MAC
#else
	#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
	#include <WinSock2.h>
	#pragma comment(lib, "wsock32.lib")
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>
#endif


class Server {
	public:
		Server() {
			if (!initSockets()) {
				throw std::runtime_error("Failed to initialize sockets.");
			}


		}

		~Server() {
			stopSockets();
		}

	private:
		bool initSockets() {
			#if PLATFORM == PLATFORM_WINDOWS
				WSADATA WsaData; // Contains information about the socket implementation
				// Initiates use of the WinSock DLL, must be the first winsock function called
				return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
			#else
				return true;
			#endif
		}

		void stopSockets() {
			#if PLATFORM == PLATFORM_WINDOWS
				WSACleanup(); // Terminates use of the WinSock DLL
			#endif
		}
};

int main()
{
    std::cout << "Starting...\n";
	try {
		Server server;
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what();
	}
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
