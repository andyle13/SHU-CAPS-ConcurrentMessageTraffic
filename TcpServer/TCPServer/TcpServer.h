#undef	UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "ThreadPool.h"
#pragma comment (lib, "ws2_32.lib")

class TcpServer
{
public:
	TcpServer(const char* ipAddress, const char* port)
	: ipv4(ipAddress), port(port) {}
	
	// Initialise the listening socket
	int Initialise();

	// Run the listening socket
	int Run();
	
protected:
	// Handler for client connections
	virtual void OnClientConnected(const SOCKET& targetSocket) = 0;

	// Handler for client disconnections
	virtual void OnClientDisconnected(const SOCKET& targetSocket) = 0;

	// Handler for received messages
	virtual void OnMessageReceived(const SOCKET& targetSocket, const char* msg) = 0;

	// Send a message to the client
	void SendToClient(const SOCKET& clientSocket, const char* msg, const unsigned int& length);

	// Initiate client disconnection
	void Disconnect(const SOCKET& clientSocket);
	
private:
	void ThreadClient(const SOCKET& client);
	
	const char*		ipv4;
	const char*		port;
	SOCKET			listener;
};

