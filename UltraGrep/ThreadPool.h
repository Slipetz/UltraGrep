#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <vector>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <future>
#include <atomic>
using namespace std;
#include <Windows.h>

#include "IWorker.h"
#include "GrepResults.h"

class ThreadPool {
public:
	//Typedefs
	using worker_type = unique_ptr<IWorker>;

	//Data Members
	unsigned int				numOfThreads;
	vector<thread>				threadVector;
	queue<worker_type>			workerQueue;

	//Lock Items
	mutex						queueLock;
	condition_variable			queueNotify;
	atomic<bool>				threadPoolStopped;

	//Results Mutex
	mutex						resultsLock;

	//Results/Futures
	vector<future<void>>		futuresThread;
	vector<GrepResults>			threadResults;

public:
	//Constructor 0-Arg - Defaults Thread amount to number of processors on machine
	ThreadPool();

	//Constructor - 1-arg - Allows us to specify the pool size
	ThreadPool(size_t const& poolSize); 

	//Object Destructor - Ensures we have ended all the threads, in case we didn't explicitly tell the queue to stop
	~ThreadPool();

	void EnqueueWorker(worker_type& worker);
	void AssignWorker();

	//Temporarily Void - Need to create a Class to hold the results
	void StopPool();

	vector<GrepResults> getResults();

};

#endif