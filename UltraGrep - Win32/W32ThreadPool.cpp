#include <iostream>
using namespace std;
#include "GrepResults.h"
#include "W32ThreadPool.h"

//WORK CALLBACK FUNCTION - What gets called on the thread!
VOID CALLBACK PerformWork(PTP_CALLBACK_INSTANCE instance, void * context) {
	UNREFERENCED_PARAMETER(instance);

	W32ThreadPool& tPool = *(reinterpret_cast < W32ThreadPool* > (context));
	W32ThreadPool::worker_type worker;
	worker = std::move(tPool.GetNextJob());

	if (worker != nullptr) {
		GrepResults results = worker->doWork();
		if (results.getTotalMatches() > 0) {
			tPool.AddResult(results);
		}
	}
	worker.reset();
}


W32ThreadPool::W32ThreadPool()
{
	cout << "Using W32 ThreadPool..." << endl;
	InitializeThreadpoolEnvironment(&callbackEnvironment);
	threadPool = CreateThreadpool(NULL);
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	auto nThreads = si.dwNumberOfProcessors;
	SetThreadpoolThreadMaximum(threadPool, nThreads);
	cleanupGroup = CreateThreadpoolCleanupGroup();

	//Associates the Callbacks with this pool
	SetThreadpoolCallbackPool(&callbackEnvironment, threadPool);
	//Associates Callbacks to that environment to that cleanup group
	SetThreadpoolCallbackCleanupGroup(&callbackEnvironment, cleanupGroup, NULL);
}

W32ThreadPool::W32ThreadPool(DWORD const& nThreads)
{
	cout << "Using W32 ThreadPool..." << endl;
	InitializeThreadpoolEnvironment(&callbackEnvironment);
	threadPool = CreateThreadpool(NULL);
	SetThreadpoolThreadMaximum(threadPool, nThreads);
	cleanupGroup = CreateThreadpoolCleanupGroup();

	//Associates the Callbacks with this pool
	SetThreadpoolCallbackPool(&callbackEnvironment, threadPool);
	//Associates Callbacks to that environment to that cleanup group
	SetThreadpoolCallbackCleanupGroup(&callbackEnvironment, cleanupGroup, NULL);
}

W32ThreadPool::~W32ThreadPool()
{
	CloseThreadpoolCleanupGroupMembers(cleanupGroup, FALSE, NULL);
	CloseThreadpoolCleanupGroup(cleanupGroup);
	CloseThreadpool(threadPool);
	DestroyThreadpoolEnvironment(&callbackEnvironment);
}

void W32ThreadPool::EnqueueWorker(worker_type& worker)
{
	{
		W32Lock qLock(queueCS);
		workerQueue.push(std::move(worker));
		TrySubmitThreadpoolCallback(PerformWork, this, &callbackEnvironment);
	}
}

void W32ThreadPool::AddResult(GrepResults const& result)
{
	{
		W32Lock resLock(resultsCS);
		results.emplace_back(result);
	}
}

vector<GrepResults> W32ThreadPool::getResults()
{
	while (!workerQueue.empty()) { Sleep(25); }
	CloseThreadpoolCleanupGroupMembers(cleanupGroup, FALSE, NULL);
	{
		W32Lock resLock(resultsCS);
		return results;
	}
}

W32ThreadPool::worker_type W32ThreadPool::GetNextJob()
{
	worker_type wkr = nullptr;
	{
		W32Lock qLock(queueCS);
		if (!workerQueue.empty()) {
			wkr = std::move(workerQueue.front());
			workerQueue.pop();
		}
	}
	return wkr;
}




