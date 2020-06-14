#include "DataRepository.h"

std::string DataRepository::Post(const std::string& topic, std::string message)
{
	if(message.length() > 0)
	{
		// Truncate message if character count exceeds 140
		if (message.length() > 140)
			message = message.substr(0, 140);

		myMutex.lock();
		data[topic].emplace_back(message);
		myMutex.unlock();
		
		return std::to_string(data[topic].size() - 1);
	}
	
	return "Post failed.";
}

std::string DataRepository::List()
{
	if (!data.empty())
	{
		unsigned int i(0);
		std::string topicIds;

		for (const auto& topic : data) {
			topicIds += topic.first;
			if (i < data.size() - 1)
				topicIds += "#";

			++i;
		}

		return topicIds;
	}

	return {};
}

std::string DataRepository::Read(const std::string& topic, const unsigned int& id)
{
	std::string result({});

	//if(IsTopicKeyExistentiallyValid(topic))
	//{
	//	/* Usually items are inserted towards the end of the map
	//	 * but accessing an index without an entry would increase
	//	 * the size of an unordered_map. Therefore it is crucial
	//	 * to check the id against the size since id allocation
	//	 * occurs in order as they are inserted by size.
	//	 */
	myMutex.lock_shared();
	if (data[topic].size() > id)
	{
		if (!data[topic].at(id).empty())
			result = data[topic].at(id);
	}
	myMutex.unlock_shared();
	//}
	return result;
}

std::string DataRepository::Count(const std::string& topic)
{
	//if(IsTopicKeyExistentiallyValid(topic))
	//{
		if (!data[topic].empty())
			return std::to_string(data[topic].size());
	//}

	return "0";
}

//bool DataRepository::IsTopicKeyExistentiallyValid(const std::string& topic)
//{
//	/* Checks if topic key exists in the unordered map
//	 * to prevent the insertion of a non-existent key
//	 */
//	return data.find(topic) != data.end();
//}