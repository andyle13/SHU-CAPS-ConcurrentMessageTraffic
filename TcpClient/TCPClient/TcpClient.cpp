#include "TcpClient.h"

int TcpClient::Initialise()
{
	WSAData wsaData;

	struct addrinfo	* result = NULL,
					* ptr = NULL,
					hints;
	
	// WinSock initialisation
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	if (getaddrinfo(m_ipAddress, m_port, &hints, &result) != 0) {
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		m_socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (m_socket == INVALID_SOCKET) {
			WSACleanup();
			return 1;
		}

		//Connect to server.				
		if (connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);
	
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return 1;
	}

	return 0;
}

double TcpClient::Run(std::string_view request, MockRepository* data)
{
	auto thrput = CalculateThroughput(request, data);
	
	// Terminate client socket
	closesocket(m_socket);
	WSACleanup();
	return thrput;
}

void TcpClient::SendToServer(const SOCKET& serverSocket, const char* msg, int length)
{
	// Send message
	if (send(serverSocket, msg, length, 0) != SOCKET_ERROR)
	{
		char buf[4096];

		// Await reply
		ZeroMemory(buf, 4096);
		if (recv(m_socket, buf, 4096, 0) >= 0)
		{
			// Output to console
			//buf[strcspn(buf, "\r\n")] = 0;
			//OnMessageReceived(m_socket, buf);
		}
	}
}

double TcpClient::CalculateThroughput(std::string_view threadType, MockRepository * data)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> exeTime;
	// Message count to be divided by number of total threads
	while (true)
	{
		// Measure the throughput time from start to end within 10 seconds
		exeTime = std::chrono::high_resolution_clock::now() - start;
		if (exeTime.count() >= 10)
			break;

		OnMessageSending(m_socket, threadType, data);
		++requestFulfilled;
	}

	// Once Sending is completed, send signal to terminate program
	auto thrput = requestFulfilled / exeTime.count();
	printf("Throughput of Thread %d:\t%s msg/sec\n", std::this_thread::get_id(), std::to_string(thrput));
	return thrput;
}
