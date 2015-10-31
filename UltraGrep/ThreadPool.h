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

#include "IThreadPool.h"


//ThreadPool - Concrete Instance of the IThreadPool interface. 
//Uses the C++11 <thread> library to create and manage a ThreadPool
//Date - Oct 25, 2015
class ThreadPool : public IThreadPool {
public:

	//WorkerQueue - Holds the jobs until the thread is ready to handle them!
	queue<worker_type>			workerQueue;

	//Queue Lock Items - Mutex/Notify threads + "flag" variable to tell threads when they can stop running
	mutex						queueLock;
	condition_variable			queueNotify;
	atomic<bool>				threadPoolStopped;

	//Results Mutex/
	mutex						resultsLock;

	//Results/Futures
	vector<future<void>>		futuresThread;
	vector<GrepResults>			threadResults;

public:
	//Constructor 0-Arg - Defaults Thread amount to number of processors on machine - uses "thread::hardware_processes()"
	ThreadPool();

	//Constructor - 1-arg - Allows user to specify the size they want their threadpool to start at
	ThreadPool(size_t const& poolSize); 

	//Object Destructor - Ensures we have ended all the threads, in case we didn't explicitly tell the queue to stop
	~ThreadPool();

	//IThreadPool - Implemented virtual functions from the base class
	void EnqueueWorker(worker_type& worker) override;
	vector<GrepResults> getResults() override;


	//DoThreadTasks
	//Purpose - Member Function that gets attached to each thread running Async.
	//		  - Will attempt to pull from worker queue if not empty() until stop is called
	//Returns - Nothing. Results are passed during the function through a lock
	//Accepts - Nothing. Everything needed for the task exists within the function + attached threadpool
	void DoThreadTasks();

};

#endif