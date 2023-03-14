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

	// Waits until threads finish their current tasks;
	void Wait();

	void PrintToConsole(const std::string& mesg);

	// Modifies the values which serve as a state keeper of how the tasks of the threads are going
	void AddTaskGiven();
	void AddTaskFinished();
	void ResetFinishedTasks();
	void SetTaskCount(int count);
	bool HasFinished();
	
	// Submit a function to be executed asynchronously by the pool
	template<typename F, typename...Args>
	auto Submit(F&& f, Args&&... args)->std::future<decltype(f(args...))>;

	template<typename F, class T, class T1, typename...Args>
	auto Submit(F T::* f, T1* obj, Args&&... args)->std::future<decltype((std::declval<T>().*f)(args...))>;

	size_t ThreadCount();
	std::thread::id GetMainThreadID();

private:
	ThreadPoolClass() : mbShutDown(false), mGivenTasks(0), mFinishedTasks(0) {}

	bool mbShutDown;
	int mGivenTasks;
	int mFinishedTasks;
	std::thread::id mMainThread;
	TaskQueue<std::function<void()>> mQueue;
	std::vector<std::thread> mThreads;
	std::mutex mConditionalMutex;
	std::condition_variable mConditionalLock;
};

class Worker {
public:
	Worker(const int id);
	void operator()();

private:
	int mID;
	bool mbWorking;
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
	
	////notifying a task was added
	//AddTaskGiven();

	// Wake up one thread if its waiting
	mConditionalLock.notify_one();
	
	// Return future from promise
	return taskPtr->get_future();
}

template<typename F, class T, class T1, typename...Args>
auto ThreadPoolClass::Submit(F T::* f, T1* obj, Args&&... args) -> std::future<decltype((std::declval<T>().*f)(args...))>
{
	// Create a function with bounded parameters ready to execute
	std::function<decltype((std::declval<T>().*f)(args...))()> func = [=]() { (obj->*f)(args...); };
	
	// Encapsulate it into a shared ptr in order to be able to copy construct / assign
	auto task_ptr = std::make_shared<std::packaged_task<decltype((std::declval<T>().*f)(args...))()>>(func);
	
	// Wrap packaged task into void function
	std::function<void()> wrapper_func = [task_ptr]() {
		(*task_ptr)();
	};
	
	// Enqueue generic wrapper function
	mQueue.Enqueue(wrapper_func);
	
	////notifying a task was added
	//AddTaskGiven();

	// Wake up one thread if its waiting
	mConditionalLock.notify_one();
	
	// Return future from promise
	return task_ptr->get_future();
}


#pragma endregion

#define ThreadPool (ThreadPoolClass::GetInstance())

#endif // MULTITHREAD


