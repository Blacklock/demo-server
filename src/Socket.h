#pragma once

// Platform detection

#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC 2
#define PLATFORM_UNIX 3

#ifdef _WIN32
	#define PLATFORM PLATFORM_WINDOWS
#elif __APPLE__
	#define PLATFORM PLATFORM_UNIX
#else
	#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "ws2_32.lib") // Previously wsock32.lib
#elif PLATFORM == PLATFORM_UNIX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>
#endif


struct InPacketInfo {
	int buffer_size;
	unsigned long sender_address;
	unsigned short sender_port;
};

class Socket {
private:
	SOCKET handle;
	unsigned int MAX_PACKET_SIZE;
public:
	Socket(unsigned int max_packet_size);
	~Socket();

	void sendPacket(const unsigned char packet[], unsigned short packet_size, unsigned long destIp, unsigned short port);
	InPacketInfo receivePacket(unsigned char* outBuffer);
	void create(unsigned short port);
};
