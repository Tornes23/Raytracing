#pragma once
#ifdef MULTITHREAD
#include <vector>
#include <thread>


class ThreadPoolClass
{
public:
	void InitThread();

	//singleton stuff
	ThreadPoolClass(ThreadPoolClass const&) = delete;
	void operator=(ThreadPoolClass const&) = delete;
	static ThreadPoolClass& GetInstance()
	{
		static ThreadPoolClass instance;
		return instance;
	}

private:
	ThreadPoolClass() {}
	std::vector<std::thread> mThreadPool;
};

#define ThreadPool (ThreadPoolClass::GetInstance())

#endif // MULTITHREAD
