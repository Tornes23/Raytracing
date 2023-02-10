#pragma once
#ifdef MULTITHREAD
#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "TaskQueue.h"

class ThreadPoolClass {
	friend class Worker;

public:

	//singleton stuff
	ThreadPoolClass(ThreadPoolClass const&) = delete;
	void operator=(ThreadPoolClass const&) = delete;
	ThreadPoolClass(ThreadPoolClass&&) = delete;
	ThreadPoolClass& operator=(ThreadPoolClass&&) = delete;

	static ThreadPoolClass& GetInstance()
	{
		static ThreadPoolClass instance;
		return instance;
	}

	// Inits thread pool
	void Init();

	// Waits until threads finish their current task and shutdowns the pool;
	void ShutDown();

	// Submit a function to be executed asynchronously by the pool
	template<typename F, typename...Args>
	auto Submit(F&& f, Args&&... args)->std::future<decltype(f(args...))>;

private:
	ThreadPoolClass() : mbShutDown(false) {}

	bool mbShutDown;
	TaskQueue<std::function<void()>> mQueue;
	std::vector<std::thread> mThreads;
	std::mutex mConditionalMutex;
	std::condition_variable mConditionalLock;
};

class Worker {
public:
	Worker(const int id);

private:
	int mID;
	void operator()();
};

#pragma region Submit

template<typename F, typename ...Args>
inline auto ThreadPoolClass::Submit(F&& f, Args && ...args) -> std::future<decltype(f(args ...))>
{
	// Create a function with bounded parameters ready to execute
	std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	// Encapsulate it into a shared ptr in order to be able to copy construct / assign 
	auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

	// Wrap packaged task into void function
	std::function<void()> wrapperFunc = [taskPtr]() {
		(*taskPtr)();
	};

	// Enqueue generic wrapper function
	mQueue.Enqueue(wrapperFunc);

	// Wake up one thread if its waiting
	mConditionalLock.notify_one();

	// Return future from promise
	return taskPtr->get_future();
}

#pragma endregion

#define ThreadPool (ThreadPoolClass::GetInstance())

#endif // MULTITHREAD


