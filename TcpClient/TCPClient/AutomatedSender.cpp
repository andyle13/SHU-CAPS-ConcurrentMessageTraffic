#include "AutomatedSender.h"

void AutomatedSender::OnServerConnected(const SOCKET& targetSocket)
{
	// Confirm connection message
	std::string msgConfirm("Connection successful.");
	SendToServer(targetSocket, msgConfirm.c_str(), msgConfirm.size() + 1);
}

void AutomatedSender::OnServerDisconnected(const SOCKET& targetSocket)
{
	// Confirm disconnection message
	std::string msgConfirm("Disconnected.");
	SendToServer(targetSocket, msgConfirm.c_str(), msgConfirm.size() + 1);
}

void AutomatedSender::OnMessageSending(const SOCKET& targetSocket, std::string_view threadType, MockRepository* data)
{
	// Determine the message type based on the thread type passed in
	std::string msg = threadType == "POST" ? data->ConstructMessagePoster() : data->ConstructMessageReader();

	// UNCOMMENT FOR MESSAGES
	//std::cout << "Message sent: " << std::string_view(msg) << std::endl;
	//std::cout << "Bytes sent: " << std::string_view(msg).size() << std::endl;

	SendToServer(targetSocket, msg.c_str(), msg.size() + 1);
}

void AutomatedSender::OnMessageReceived(const SOCKET& targetSocket, const char* msg)
{
	// UNCOMMENT FOR MESSAGES
	//std::cout << "SERVER> " << msg << std::endl;
}