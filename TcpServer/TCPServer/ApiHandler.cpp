#include "ApiHandler.h"

void ApiHandler::OnClientConnected(const SOCKET& targetSocket)
{
	printf("Socket #%d is online.\n", targetSocket);
	std::string msgConfirm("Connection successful.");
	SendToClient(targetSocket, msgConfirm.c_str(), msgConfirm.size() + 1);
}

void ApiHandler::OnClientDisconnected(const SOCKET& targetSocket)
{
	printf("Socket #%d is offline.\n", targetSocket);
	Disconnect(targetSocket);
}

void ApiHandler::OnMessageReceived(const SOCKET& targetSocket, const char* msg)
{
	//	UNCOMMENT FOR SEEING MESSAGE
	//std::cout << "Socket #" << std::to_string(targetSocket) << " posted: " << msg << std::endl;
	
	const auto strOut = RequestRouter(msg);

	SendToClient(targetSocket, strOut.c_str(), strOut.size() + 1);
}

std::string ApiHandler::RequestRouter(const std::string& request)
{
	std::smatch match;

	if (std::regex_match(request, match, std::regex("^POST(@[^@#]*)#(.+)$"), std::regex_constants::match_default))
		return Post(match[1], match[2]);
	if (std::regex_match(request, match, std::regex("^READ(@[^@#]*)#([0-9]+)$"), std::regex_constants::match_default))
		return Read(match[1], std::stoi(match[2]));
	if (std::regex_match(request, std::regex("^LIST$"), std::regex_constants::match_default))
		return List();
	if (std::regex_match(request, match, std::regex("^COUNT(@[^@#]*)$"), std::regex_constants::match_default))
		return Count(match[1]);

	return {};
}