#ifdef MULTITHREAD
#include "ThreadPool.h"
#include <Windows.h>
#include <iostream>

void ThreadPoolClass::Init() {

	mGivenTasks = 0;
	mFinishedTasks = 0;
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

void ThreadPoolClass::Wait()
{
	while (mFinishedTasks < mGivenTasks)
		Sleep(10);
}

void ThreadPoolClass::AddTaskGiven(){ 
	std::lock_guard<std::mutex> lock(mConditionalMutex);
	mGivenTasks++; 
}

void ThreadPoolClass::AddTaskFinished() { 
	std::lock_guard<std::mutex> lock(mConditionalMutex);
	mFinishedTasks++; 
	std::cout << "Finished Task count = " << mFinishedTasks << "\n";
}

void ThreadPoolClass::ResetFinishedTasks() { mFinishedTasks = 0; }

void ThreadPoolClass::SetTaskCount(int count) { mGivenTasks = count; std::cout << "Task count = " << mGivenTasks << "\n"; }

int ThreadPoolClass::ThreadCount() { return mThreads.size(); }


Worker::Worker(const int id) : mID(id), mbWorking(false) { }

void Worker::operator()()
{
	std::function<void()> func;
	bool dequeued;
	while (!ThreadPool.mbShutDown) {
		
		{
			std::unique_lock<std::mutex> lock(ThreadPool.mConditionalMutex);
			mbWorking = false;

			while (ThreadPool.mQueue.Empty()) {
				ThreadPool.mConditionalLock.wait(lock);

			}
			dequeued = ThreadPool.mQueue.Dequeue(func);
			mbWorking = true;
		}

			if (dequeued) {
				ThreadPool.AddTaskGiven();
				func();
			}
	}
}

#endif // MULTITHREAD