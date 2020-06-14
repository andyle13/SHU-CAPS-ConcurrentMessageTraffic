#pragma once
#include "TcpServer.h"
#include "DataRepository.h"
#include <string_view>
#include <iostream>
#include <regex>

class ApiHandler : public TcpServer, DataRepository
{
public:
	ApiHandler(const char* ipAddress, const char* port)
	: TcpServer(ipAddress, port) {}

protected:
	// Handler for client connections
	virtual void OnClientConnected(const SOCKET& targetSocket);

	// Handler for client disconnections
	virtual void OnClientDisconnected(const SOCKET& targetSocket);

	// Handler for received messages
	virtual void OnMessageReceived(const SOCKET& targetSocket, const char* msg);

private:
	// Routes Request
	std::string RequestRouter(const std::string& request);
};

