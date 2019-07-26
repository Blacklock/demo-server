#include "Socket.h"
#include "GameServer.h"

#include <iostream>
#include <string>


Socket::Socket() {
	// Initialize the WinSock DLL
	
	#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData; // Contains information about the socket implementation

		if (WSAStartup(MAKEWORD(2, 2), &WsaData) != NO_ERROR) { // Must be first winsock func called on windows
			throw std::exception("Failed to initialize the WinSock DLL.");
		}
	#endif
}

Socket::~Socket() {
	#if PLATFORM == PLATFORM_UNIX
		close(handle);
	#elif PLATFORM == PLATFORM_WINDOWS
		closesocket(handle);
		WSACleanup(); // Terminates use of the WinSock DLL
	#endif
}

void Socket::sendPacket(const char packet[], const char destIp[46], unsigned short port) {
	sockaddr_in address;

	if (inet_pton(AF_INET, destIp, &(address.sin_addr)) != 1) { // perhaps sin_addr.s_addr ?????
		throw std::exception("IP address conversion failed.");
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	//size_t packet_size = strlen(packet);
	unsigned int packet_size = (unsigned int)strlen(packet);

	int sent_bytes = sendto(handle, (const char*)packet, packet_size, 0, (sockaddr*)&address, sizeof(address));

	if (sent_bytes != packet_size) {
		throw std::exception("Failed to send packet.");
	}
}

int Socket::receivePacket(unsigned char* out_buffer) {
	//while (true) {
	// unsigned char packet[256];

	#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
	#endif

	sockaddr_in sender; // Will hold the sender's address
	socklen_t senderLength = sizeof(sender);

	int received_bytes = recvfrom(handle, (char*)out_buffer, max_packet_size, 0, (sockaddr*)&sender, &senderLength);

	//if (received_bytes <= 0) break; // No more packets to receive

	unsigned int sender_address = ntohl(sender.sin_addr.s_addr);
	unsigned int sender_port = ntohl(sender.sin_port);

	/*if (received_bytes > 0) {
		memcpy(out_buffer, packet, received_bytes);
	}*/

	return received_bytes;
	//}

	//return -1;
}

void Socket::create(unsigned short port) {
	if (port != 0 && (port < 2000 || port > 45000)) {
		throw std::out_of_range("Port number out of allowed range (0 || 2000-45000).");
	}

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
			if (ioctlsocket(handle, FIONBIO, &nonblocking) == SOCKET_ERROR) {
				throw std::exception("Failed to set socket to non-blocking mode.");
			}
	#elif PLATFORM == PLATFORM_UNIX
			int nonblocking = 1;
			if (fcntl(handle, F_SETFL, O_NONBLOCK, nonblocking) == -1) {
				throw std::exception("Failed to set socket to non-blocking mode.");
			}
	#endif
}
