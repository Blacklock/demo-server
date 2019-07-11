// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>

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
	#include <ws2tcpip.h>
	#pragma comment(lib, "ws2_32.lib") // Previously wsock32.lib
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>
#endif


class Server {
	public:
		int handle;
		unsigned int max_packet_size = 256;

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

		bool sendPacket(const char packet[], const char destIp[INET_ADDRSTRLEN], unsigned short port) {
			sockaddr_in address;

			if (inet_pton(AF_INET, destIp, &(address.sin_addr)) != 1) { // perhaps sin_addr.s_addr ?????
				throw std::exception("IP address conversion failed.");
			}

			address.sin_family = AF_INET;
			address.sin_port = htons(port);

			size_t packet_size = strlen(packet);

			int sent_bytes = sendto(handle, (const char*)packet, packet_size, 0, (sockaddr*)&address, sizeof(address));

			if (sent_bytes != packet_size) {
				throw std::exception("Failed to send packet.");
			}

			return true;
		}

		void receivePackets() {
			std::cout << "Receiving packets.\n";

			while (true) {
				unsigned char packet[256];

				#if PLATFORM == PLATFORM_WINDOWS
					typedef int socklen_t;
				#endif

				sockaddr_in sender; // Will hold the sender's address
				socklen_t senderLength = sizeof(sender);

				int received_bytes = recvfrom(handle, (char*)packet, max_packet_size, 0, (sockaddr*)&sender, &senderLength);

				if (received_bytes <= 0) break; // No more packets to receive

				unsigned int sender_address = ntohl(sender.sin_addr.s_addr);
				unsigned int sender_port = ntohl(sender.sin_port);

				std::cout << "Received " << received_bytes << " bytes.\n";
				
				for (int i = 0; i < received_bytes; i++) {
					std::cout << packet[i];
				}

				std::cout << "\n";
			}

			std::cout << "All packets received.\n";
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
			handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // IPv4 and UDP

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

			// Set to non-blocking mode

			#if PLATFORM == PLATFORM_WINDOWS
				DWORD nonblocking = 1;
				if (ioctlsocket(handle, FIONBIO, &nonblocking) == SOCKET_ERROR) { // Disables blocking mode
					throw std::exception("Failed to set socket to non-blocking mode.");
				}
			#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
				int nonblocking = 1;
				if (fcntl(handle, F_SETFL, O_NONBLOCK, nonblocking) == -1) { // Disablex blocking mode
					throw std::exception("Failed to set socket to non-blocking mode.");
				}
			#endif

			return true;
		}
};

int main()
{
    std::cout << "Starting...\n";
	try {
		Server server = Server(25445);

		// Send a test socket (temp)
		//char destIp[INET_ADDRSTRLEN] = "127.0.0.1";
		/*server.sendPacket("Hello, world!", "127.0.0.1", 25444);
		server.sendPacket("Another message...", "127.0.0.1", 25444);
		server.sendPacket("...and the last one.", "127.0.0.1", 25444);*/

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


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
