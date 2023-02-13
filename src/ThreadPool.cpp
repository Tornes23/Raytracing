#ifdef MULTITHREAD
#include "ThreadPool.h"
#include <iostream>

void ThreadPoolClass::Init() {

	int count = std::thread::hardware_concurrency() - 1;
	mThreads = std::vector<std::thread>(count);
	mbShutDown = false;
	for (int i = 0; i < mThreads.size(); ++i) {
		mThreads[i] = std::thread(Worker(i));
	}
}


void ThreadPoolClass::ShutDown(){

	mbShutDown = true;
	mConditionalLock.notify_all();
	
	for (int i = 0; i < mThreads.size(); i++) 
		if (mThreads[i].joinable()) mThreads[i].join();
}

int ThreadPoolClass::ThreadCount() { return mThreads.size(); }


Worker::Worker(const int id) : mID(id) { }

void Worker::operator()()
{
	std::function<void()> func;
	bool dequeued;
	while (!ThreadPool.mbShutDown) {
		{
			std::unique_lock<std::mutex> lock(ThreadPool.mConditionalMutex);
			if (ThreadPool.mQueue.Empty()) {
				ThreadPool.mConditionalLock.wait(lock);
			}
			dequeued = ThreadPool.mQueue.Dequeue(func);
		}
		if (dequeued) {
			func();
		}
	}
}

#endif // MULTITHREAD