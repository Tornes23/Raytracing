#ifdef MULTITHREAD
#include "ThreadPool.h"

void ThreadPoolClass::Init(int poolsize)
{
	mThreadPool.resize(poolsize);
}

#endif // MULTITHREAD