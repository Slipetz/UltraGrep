#ifndef __W32THREADPOOl__
#define __W32THREADPOOL__

#include <memory>
#include <vector>
#include <queue>
using namespace std;
#include <Windows.h>

#include "IThreadPool.h"
#include "W32CSLock.h"

//W32ThreadPool - Concrete Instance of the IThreadPool interface. 
//Provides a wrapper around a Win32 ThreadPool object to allow for easy use of Win32 Threading
//Date - Oct 30, 2015
class W32ThreadPool : public IThreadPool {
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
	vector<GrepResults> results;

	//OOP Win32 Mutex Objects
	CritSection queueCS;
	CritSection resultsCS;

	//Queue of Jobs to be assigned/grabbed by each "Work Object" on the threadpool
	queue<worker_type> workerQueue;

public:
	//0-Arg Constructor - Uses SYSTEM_INFO to determine max number of Threads that should be used
	//NOTE: May not be necessary to cap. Windows will manage the number of threads needed. We may be killing performance here...
	W32ThreadPool();

	//1-Arg Constructor - Takes a size_t object to determine how max number of threads that should be used
	//NOTE: May not be necessary to cap. Windows will manage the number of threads needed. We may be killing performance here...
	W32ThreadPool(DWORD const&);

	//Destructor - Cleans up all of the Win32 ThreadPool stuff so we don't leave any environment objects left over!
	~W32ThreadPool();

public:
	//EnqueueWorker - Overridden method from the base class - PDL is there!
	void EnqueueWorker(worker_type& worker) override;

	//AddResult
	//Purpose - Adds a result to the result vector - utilizes a lock here so we threads cannot override/collide with each other
	//Accepts - Result object to be added to the vector
	//Returns - Nothing! GrepResult is added to the vector in the threadpool
	void AddResult(GrepResults const& result);

	//GetResults - Overriden method from the base class - PDL Provided there!
	vector<GrepResults> getResults() override;

	//GetNextJob
	//Purpose - Grab the next IWorker object off of the top of the Queue through the Win32 Lock
	//Accepts: Nothing!
	//Returns: A worker_type object from the top of the Queue
	worker_type GetNextJob();
};


#endif