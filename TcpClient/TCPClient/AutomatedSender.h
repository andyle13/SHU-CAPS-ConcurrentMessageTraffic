#pragma once
#include "TcpClient.h"
#include <regex>
#include <string_view>

class AutomatedSender : public TcpClient
{
public:
	AutomatedSender(const char* ipAddress, const char* port)
	: TcpClient(ipAddress, port) {}

protected:
	// Handler for server connections
	virtual void OnServerConnected(const SOCKET& targetSocket);

	// Handler for server disconnections
	virtual void OnServerDisconnected(const SOCKET& targetSocket);

	// Handler for sending messages
	virtual void OnMessageSending(const SOCKET& targetSocket, std::string_view threadType, MockRepository * data);
	
	// Handler for received messages
	virtual void OnMessageReceived(const SOCKET& targetSocket, const char* msg);
};

