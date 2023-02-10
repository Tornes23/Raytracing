#pragma once

#ifdef MULTITHREAD
#include <queue>
#include <mutex>

template<typename T>
class TaskQueue
{
public:
	bool Empty();
	int  Size();
	void Enqueue(T& t);
	bool Dequeue(T& t);

private:
	std::queue<T> mQueue;
	std::mutex mMutex;
};

template<typename T>
bool TaskQueue<T>::Empty(){
	std::unique_lock<std::mutex> lock(mMutex);
	return mQueue.empty();
}

template<typename T>
int  TaskQueue<T>::Size() {
	std::unique_lock<std::mutex> lock(mMutex);
	return mQueue.size();
}

template<typename T>
void TaskQueue<T>::Enqueue(T& t) {
	std::unique_lock<std::mutex> lock(mMutex);
	mQueue.push(t);
}

template<typename T>
bool TaskQueue<T>::Dequeue(T& t) {
	std::unique_lock<std::mutex> lock(mMutex);
	if (mQueue.empty()) return false;
	t = std::move(mQueue.front());
	mQueue.pop();
	return true;
}

#endif // MULTITHREAD
