#pragma once
#include <condition_variable>
#include <future>
#include <functional>
#include <vector>
#include <thread>
#include <queue>

class ThreadPool
{
public:
	using Task = std::function<void()>;

	explicit ThreadPool(std::size_t maxThreads);

	~ThreadPool();

	template<class T>
	auto QueueTask(T task)->std::future<decltype(task())>
	{
		auto Wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
		{
			std::unique_lock<std::mutex> lock(eventMutex);
			taskQueue.emplace([=]
			{
				(*Wrapper)();
			});
		}

		condVar.notify_one();
		return Wrapper->get_future();
	}

	bool IsTaskQueueEmpty();

private:
	void StartTask(std::size_t maxThreads) noexcept;
	void StopTask() noexcept;

    std::vector<std::thread> threadList;
	std::queue<Task> taskQueue;
    std::condition_variable condVar;
    std::mutex eventMutex;
    bool hasTaskStopped = false;
};

