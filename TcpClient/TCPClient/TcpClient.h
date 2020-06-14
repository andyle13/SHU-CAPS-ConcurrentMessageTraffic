#define WIN32_LEAN_AND_MEAN

#include "MockRepository.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class TcpClient
{
public:
	TcpClient(const char* ipAddress, const char* port)
		: m_ipAddress(ipAddress), m_port(port) {}

	// Initialise the listening socket
	int Initialise();

	// Run the listening socket
	double Run(std::string_view req, MockRepository* data);

protected:
	// Handler for server connections
	virtual void OnServerConnected(const SOCKET& targetSocket) = 0;

	// Handler for server disconnections
	virtual void OnServerDisconnected(const SOCKET& targetSocket) = 0;

	// Handler for sending messages
	virtual void OnMessageSending(const SOCKET& targetSocket, std::string_view threadType, MockRepository* data) = 0;
	
	// Handler for received messages
	virtual void OnMessageReceived(const SOCKET& targetSocket, const char* msg) = 0;

	// Send a message to the client
	void SendToServer(const SOCKET& serverSocket, const char* msg, int length);

private:
	double CalculateThroughput(std::string_view threadType, MockRepository * data);
	int								requestFulfilled;
	const char*						m_ipAddress;
	const char*						m_port;
	int								m_socket;
};

