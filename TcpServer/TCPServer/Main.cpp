#include "ApiHandler.h"

#define DEFAULT_PORT "12345"

void main(int argc, char** argv)
{	
	ApiHandler server(argv[1], DEFAULT_PORT);

	if (server.Initialise() != 0)
		return;

	server.Run();
	
	system("pause");
}
