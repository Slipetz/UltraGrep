#ifndef __W32THREADPOOl__
#define __W32THREADPOOL__

#include <memory>
#include <vector>
#include <queue>
using namespace std;
#include <Windows.h>

#include "../UGLib/IThreadPool.h"
#include "W32CSLock.h"


//W32ThreadPool - Concrete Instance of the IThreadPool interface. 
//Provides a wrapper around a Win32 ThreadPool object to allow for easy use of Win32 Threading
//Date - Oct 30, 2015

template <typename ReturnType>
class W32ThreadPool : public IThreadPool<ReturnType> {
private:
	//Callback Environment - Force Callbacks to occur in the pool we pass this too
	TP_CALLBACK_ENVIRON callbackEnvironment;

	//ThreadPool!
	PTP_POOL threadPool;

	//CleanUp Group - Does the clean-up of the work objects we are going to create
	PTP_CLEANUP_GROUP cleanupGroup;

	//Work Object - we constantly create this and throw it back into the queue!
	PTP_WORK workObject;

	//Vector of Results!
	vector<ReturnType> results;

	//OOP Win32 Mutex Objects
	CritSection queueCS;
	CritSection resultsCS;

	//Queue of Jobs to be assigned/grabbed by each "Work Object" on the threadpool
	queue<worker_type> workerQueue;

public:
	//0-Arg Constructor - Uses SYSTEM_INFO to determine max number of Threads that should be used
	//NOTE: May not be necessary to cap. Windows will manage the number of threads needed. We may be killing performance here...
	W32ThreadPool<ReturnType>() {
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

	//1-Arg Constructor - Takes a size_t object to determine how max number of threads that should be used
	//NOTE: May not be necessary to cap. Windows will manage the number of threads needed. We may be killing performance here...
	W32ThreadPool<ReturnType>(DWORD const&) {
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

	//Destructor - Cleans up all of the Win32 ThreadPool stuff so we don't leave any environment objects left over!
	~W32ThreadPool<ReturnType>() {
		CloseThreadpoolCleanupGroupMembers(cleanupGroup, FALSE, NULL);
		CloseThreadpoolCleanupGroup(cleanupGroup);
		CloseThreadpool(threadPool);
		DestroyThreadpoolEnvironment(&callbackEnvironment);
	}

public:
	//EnqueueWorker - Overridden method from the base class - PDL is there!
	void EnqueueWorker(worker_type& worker) override {
		W32Lock qLock(queueCS);
		workerQueue.push(std::move(worker));
		TrySubmitThreadpoolCallback(PerformWork, this, &callbackEnvironment);
	}

	//AddResult
	//Purpose - Adds a result to the result vector - utilizes a lock here so we threads cannot override/collide with each other
	//Accepts - Result object to be added to the vector
	//Returns - Nothing! GrepResult is added to the vector in the threadpool
	void AddResult(GrepResults const& result) {
		W32Lock resLock(resultsCS);
		results.emplace_back(result);
	}

	//GetResults - Overriden method from the base class - PDL Provided there!
	vector<ReturnType> getResults() override {
		while (!workerQueue.empty()) { Sleep(25); }
		CloseThreadpoolCleanupGroupMembers(cleanupGroup, FALSE, NULL);
		{
			W32Lock resLock(resultsCS);
			return results;
		}
	}

	//GetNextJob
	//Purpose - Grab the next IWorker object off of the top of the Queue through the Win32 Lock
	//Accepts: Nothing!
	//Returns: A worker_type object from the top of the Queue
	worker_type GetNextJob() {
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

	static VOID CALLBACK PerformWork(PTP_CALLBACK_INSTANCE instance, void * context) {
		UNREFERENCED_PARAMETER(instance);

		W32ThreadPool<ReturnType>& tPool = *(reinterpret_cast < W32ThreadPool<ReturnType>* > (context));
		W32ThreadPool::worker_type worker;
		if(!tPool.workerQueue.empty())
			worker = std::move(tPool.GetNextJob());

		if (worker != nullptr) {
			ReturnType results = worker->doWork();
			tPool.AddResult(results);
		}
		worker.reset();
	}

};


#endif