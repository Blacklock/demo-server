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
		Server(unsigned short port) {
			// Initialize the WinSock DLL
			initSockets();
			std::cout << "Initialized sockets...\n";

			// Create a socket, bind it to a port, and set it to non-blocking mode
			createSocket(port);
			std::cout << "Created a socket...\n";


		}

		~Server() {
			stopSockets();
		}

	private:
		bool initSockets() {
			#if PLATFORM == PLATFORM_WINDOWS
				WSADATA WsaData; // Contains information about the socket implementation

				if (WSAStartup(MAKEWORD(2, 2), &WsaData) != NO_ERROR) { // This must be the first winsock function called
					throw std::exception("Failed to initialize the WinSock DLL.");
				}
			#endif

			return true;
		}

		void stopSockets() {
			#if PLATFORM == PLATFORM_WINDOWS
				WSACleanup(); // Terminates use of the WinSock DLL
			#endif
		}

		bool createSocket(unsigned short port) {
			int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // IPv4 and UDP

			// if (handle <= 0) ...
			if (handle == INVALID_SOCKET) {
				throw std::exception("Failed to create a socket.");
			}

			// Bind the socket to a port

			sockaddr_in address;
			address.sin_family = AF_INET; // IPv4
			address.sin_addr.s_addr = INADDR_ANY; // Any incoming IP address
			address.sin_port = htons(port); // Convert int to big-endian

			if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
				throw std::exception("Failed to bind socket to a port.");
			}

			#if PLATFORM == PLATFORM_WINDOWS
				DWORD nonBlocking = 1;
				if (ioctlsocket(handle, FIONBIO, &nonBlocking) == SOCKET_ERROR) { // Disables blocking mode
					throw std::exception("Failed to set socket to non-blocking mode.");
				}
			#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
				int nonBlocking = 1;
				if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1) { // Disablex blocking mode
					throw std::exception("Failed to set socket to non_blocking mode.");
				}
			#endif

			return true;
		}
};

int main()
{
    std::cout << "Starting...\n";
	try {
		Server server = Server(25444);
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
