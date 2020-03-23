#include "Socket.h"

#include <plog/Log.h>
#include <iostream>
#include <string>


Socket::Socket(unsigned int max_packet_size) {
	MAX_PACKET_SIZE = max_packet_size;

	// Initialize the WinSock DLL
	
	#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData; // Contains information about the socket implementation

		if (WSAStartup(MAKEWORD(2, 2), &WsaData) != NO_ERROR) { // Must be first winsock func called on windows
			LOGF << "Failed to initialize the WinSock DLL";
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

// was: const char destIp[46]
void Socket::sendPacket(const unsigned char packet[], unsigned short packet_size, unsigned long destIp, unsigned short port) {
	sockaddr_in address;

	address.sin_addr.s_addr = htonl(destIp);
	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	// unsigned int packet_size = (unsigned int)strlen((char*)packet);
	int sent_bytes = sendto(handle, (const char*)packet, packet_size, 0, (sockaddr*)&address, sizeof(address));

	if (sent_bytes != packet_size) {
		LOGE << "Failed to send packet";
		throw std::exception("Failed to send packet.");
	}
}

InPacketInfo Socket::receivePacket(unsigned char* out_buffer) {
	#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
	#endif

	sockaddr_in sender; // Will hold the sender's address
	socklen_t senderLength = sizeof(sender);

	InPacketInfo packet_info;

	packet_info.buffer_size = recvfrom(handle, (char*)out_buffer, MAX_PACKET_SIZE, 0, (sockaddr*)&sender, &senderLength);
	packet_info.sender_address = ntohl(sender.sin_addr.s_addr);
	packet_info.sender_port = ntohs(sender.sin_port);

	return packet_info;
}

void Socket::create(unsigned short port) {
	if (port != 0 && (port < 2000 || port > 45000)) {
		throw std::out_of_range("Port number out of allowed range (0 || 2000-45000).");
	}

	handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // IPv4 and UDP

	// if (handle <= 0) ...
	if (handle == INVALID_SOCKET) {
		LOGF << "Failed to create socket";
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
				LOGF << "Failed to set socket to non-blocking mode.";
				throw std::exception("Failed to set socket to non-blocking mode.");
			}
	#elif PLATFORM == PLATFORM_UNIX
			int nonblocking = 1;
			if (fcntl(handle, F_SETFL, O_NONBLOCK, nonblocking) == -1) {
				LOGF << "Failed to set socket to non-blocking mode.";
				throw std::exception("Failed to set socket to non-blocking mode.");
			}
	#endif
}
