#include "MockRepository.h"

std::string MockRepository::ConstructMessagePoster()
{
	// retrieve random pair from topicIds map
	auto randomTopic = std::next(std::begin(topicIds), RandomRange(0, topicIds.size() - 1));

	// increment the number of posted message of the corresponding topic id
	++randomTopic->second;

	// construct the POST request
	return "POST@" + randomTopic->first + "#" + messages[RandomRange(0, messages.size() - 1)];
}

std::string MockRepository::ConstructMessageReader()
{
	auto randomTopic = std::next(std::begin(topicIds), RandomRange(0, topicIds.size() - 1));
	return "READ@" + randomTopic->first + "#" + std::to_string(RandomRange(0,  randomTopic->second == 0 ? 0 : randomTopic->second - 1));
}

unsigned int MockRepository::RandomRange(unsigned int min, unsigned int max)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<unsigned int> dist(min, max);
	
	return dist(mt);
}
