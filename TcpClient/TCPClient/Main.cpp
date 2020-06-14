#include "AutomatedSender.h"
#include "MockRepository.h"
#include "ThreadPool.h"
#include <chrono>

#define DEFAULT_PORT	"12345"
#define WRITE			"POST"
#define READ			"READ"

double ThreadClient(MockRepository* d, const char* ipv4, std::string mode);

void main(int argc, char** argv)
{
	double totalThrput(0);
	unsigned int cThrP, cThrR;

	std::cout << "How many poster threads?" << std::endl;
	std::cout << "> ";
	std::cin >> cThrP;

	std::cout << "How many reader threads?" << std::endl;
	std::cout << "> ";
	std::cin >> cThrR;

	std::vector<std::future<double>> tasks;
	ThreadPool clients(10);
	MockRepository * data = new MockRepository();
	
	if(cThrP > 0)
	{
		for (unsigned int i(0); i < cThrP; ++i)
		{
			tasks.emplace_back(clients.QueueTask([=]
			{
				return ThreadClient(data, argv[1], WRITE);
			}));
		}
	}

	if(cThrR > 0)
	{
		for (unsigned int i(0); i < cThrR; ++i)
		{
			tasks.emplace_back(clients.QueueTask([=]
			{
				return ThreadClient(data, argv[1], READ);
			}));
		}
	}

	for(unsigned int i(0); i < tasks.size(); ++i)
	{
		totalThrput += tasks[i].get();
	}
	
	printf("Average Total Throughput:\t%s msg/sec\n", std::to_string(totalThrput / (cThrP + cThrR)));
	system("pause");
}

double ThreadClient(MockRepository * d, const char* ipv4, std::string mode)
{
	AutomatedSender client(ipv4, DEFAULT_PORT);
	if (client.Initialise() != 0)
		return double(0);

	return client.Run(std::string_view(mode), d);
}