#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t maxThreads)
{
	StartTask(maxThreads);
}

ThreadPool::~ThreadPool()
{
	StopTask();
}

bool ThreadPool::AreThreadsActive()
{
	return taskQueue.empty();
}

void ThreadPool::StartTask(std::size_t maxThreads) noexcept
{
	for(auto i = 0u; i < maxThreads; ++i)
	{
		threadList.emplace_back([=]
		{
			while(true)
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock{ eventMutex };

					condVar.wait(lock, [=] { return hasTaskStopped || !taskQueue.empty(); });

					if (hasTaskStopped && taskQueue.empty())
						break;

					task = std::move(taskQueue.front());
					taskQueue.pop();
				}
				task();
			}
		});
	}
}

void ThreadPool::StopTask() noexcept
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		hasTaskStopped = true;
	}

	condVar.notify_all();

	for (auto& thread : threadList)
		thread.join();
}
