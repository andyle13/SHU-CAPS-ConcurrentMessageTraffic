#pragma once
#include <unordered_map>
#include <string>
#include <shared_mutex>

class DataRepository
{
protected:
	// Post
	std::string Post(const std::string& topic, std::string message);

	// List
	std::string List();
	
	// Read
	std::string Read(const std::string& topic, const unsigned int& id);

	// Count
	std::string Count(const std::string& topic);
	
private:
	// Checks if topic key exists
	//bool IsTopicKeyExistentiallyValid(const std::string& topic);
	std::shared_mutex myMutex;
	std::unordered_map<std::string, std::vector<std::string>> data;
};

