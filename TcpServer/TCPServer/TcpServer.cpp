#include "TcpServer.h"

int TcpServer::Initialise()
{
	WSADATA wsaData;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	// WinSock initialisation
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		WSACleanup();
		return 1;
	}
	
	// Socket creation
	listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listener == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// IP:Port binding to socket
	if(bind(listener, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		closesocket(listener);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	// Listening socket
	if (listen(listener, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(listener);
		WSACleanup();
		return 1;
	}
	
	return 0;
}

int TcpServer::Run()
{
	std::cout << "Server successfully launched." << std::endl;

	ThreadPool clients(10);
	
	// Determines the running state of the program
	while(SOCKET clientSocket = accept(listener, nullptr, nullptr))
	{
		if(clientSocket == INVALID_SOCKET)
		{
			closesocket(listener);
			WSACleanup();
			return 1;
		}
		
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];

		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		clients.QueueTask([=]
		{
			ThreadClient(clientSocket);
		});
	}

	// WinSock shutdown
	WSACleanup();
	return 0;
}

void TcpServer::ThreadClient(const SOCKET& client)
{
	OnClientConnected(client);
	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);

		int bytesIn(recv(client, buf, 4096, 0));
		if (bytesIn == SOCKET_ERROR)
			break;
		if (bytesIn <= 0)
			break;

		buf[strcspn(buf, "\r\n")] = 0;
		OnMessageReceived(client, buf);
	}

	// Disassociate client
	OnClientDisconnected(client);
}

void TcpServer::SendToClient(const SOCKET& clientSocket, const char* msg, const unsigned int& length)
{
	// Output stuff to user
	send(clientSocket, msg, length, 0);
}

void TcpServer::Disconnect(const SOCKET& clientSocket)
{
	closesocket(clientSocket);
}