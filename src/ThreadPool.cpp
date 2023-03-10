#ifdef MULTITHREAD
#include "ThreadPool.h"
#include <Windows.h>
#include <iostream>

void ThreadPoolClass::Init() {

	mGivenTasks = 0;
	mFinishedTasks = 0;
	mMainThread = std::this_thread::get_id();
	//DEBUG
	//std::cout << "Main thread id = " << mMainThread << "\n";

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
	if (mGivenTasks == 0) return;
	//DEBUG
	//std::cout << "Given tasks are = " << mGivenTasks << "\n";
	//std::cout << "Finished tasks are = " << mFinishedTasks << "\n";
	while (mFinishedTasks < mGivenTasks)
		Sleep(10);

	SetTaskCount(0);
	ResetFinishedTasks();

}

void ThreadPoolClass::PrintToConsole(const std::string& mesg)
{
	std::lock_guard<std::mutex> lock(mConditionalMutex);
	std::cout << mesg;
}

void ThreadPoolClass::AddTaskGiven(){ 
	std::lock_guard<std::mutex> lock(mConditionalMutex);
	mGivenTasks++;
	//DEBUG
	//std::cout << "Given tasks are = " << mGivenTasks << "\n";
}

void ThreadPoolClass::AddTaskFinished() { 
	std::lock_guard<std::mutex> lock(mConditionalMutex);
	mFinishedTasks++; 
	//DEBUG
	//std::cout << "Finished tasks are = " << mFinishedTasks << "\n";
}

void ThreadPoolClass::ResetFinishedTasks() { mFinishedTasks = 0; }

void ThreadPoolClass::SetTaskCount(int count) { mGivenTasks = count; }

bool ThreadPoolClass::HasFinished() { return mFinishedTasks >= mGivenTasks; }

int ThreadPoolClass::ThreadCount() { return mThreads.size(); }

std::thread::id ThreadPoolClass::GetMainThreadID() { return mMainThread; }


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
				func();
				ThreadPool.AddTaskFinished();
			}
	}
}

#endif // MULTITHREAD