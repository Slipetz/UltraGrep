#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <vector>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <atomic>
using namespace std;

#include "../UGLib/IThreadPool.h"


//ThreadPool - Concrete Instance of the IThreadPool interface. 
//Uses the C++11 <thread> library to create and manage a ThreadPool
//Date - Oct 25, 2015
template <typename ReturnType>
class ThreadPool : public IThreadPool<ReturnType> {
public:

	//WorkerQueue - Holds the jobs until the thread is ready to handle them!
	queue<worker_type>			workerQueue;

	//Queue Lock Items - Mutex/Notify threads + "flag" variable to tell threads when they can stop running
	mutex						queueLock;
	condition_variable			queueNotify;
	atomic<bool>				threadPoolStopped;

	//Results Mutex
	mutex						resultsLock;

	//Results/Futures
	vector<thread>				threadHolder;
	vector<ReturnType>			threadResults;

	static mutex				consoleLock;
public:
	//Constructor 0-Arg - Defaults Thread amount to number of processors on machine - uses "thread::hardware_processes()"
	ThreadPool<ReturnType>() : threadPoolStopped(false) {
		cout << "Using C++11 ThreadPool..." << endl;
		size_t numOfThreads = thread::hardware_concurrency();
		for (size_t i = 0; i < numOfThreads; ++i) {
			threadHolder.emplace_back(thread(&ThreadPool<ReturnType>::DoThreadTasks, this));
		}
	}

	//Constructor - 1-arg - Allows user to specify the size they want their threadpool to start at
	ThreadPool<ReturnType>(size_t const& poolSize) : threadPoolStopped(false) {
		cout << "Using C++11 ThreadPool..." << endl;
		for (size_t i = 0; i < poolSize; ++i) {
			threadHolder.emplace_back(thread(&ThreadPool<ReturnType>::DoThreadTasks, this));
		}
	}

	//Object Destructor - Ensures we have ended all the threads, in case we didn't explicitly tell the queue to stop
	~ThreadPool<ReturnType>() {
		threadPoolStopped = true;
		queueNotify.notify_all();
		for (auto& ft : threadHolder) {
			ft.join();
		}
	}

	//IThreadPool - Implemented virtual functions from the base class
	void EnqueueWorker(worker_type& worker) override {
		lock_guard<mutex> queueGuard(queueLock);
		workerQueue.push(std::move(worker));
		queueNotify.notify_one();
	}

	vector<ReturnType> getResults() override {
		if (!workerQueue.empty()) { this_thread::sleep_for(chrono::milliseconds(25)); }
		threadPoolStopped = true;
		queueNotify.notify_all();
		for (auto& ft : threadHolder) {
			ft.join();
		}
		//Kill the futuresThread - once they've been used, they're done!
		threadHolder.clear();

		{
			lock_guard<mutex> resultsGuard(resultsLock);
			return threadResults;
		}
	}


	//DoThreadTasks
	//Purpose - Member Function that gets attached to each thread running Async.
	//		  - Will attempt to pull from worker queue if not empty() until stop is called
	//Returns - Nothing. Results are passed during the function through a lock
	//Accepts - Nothing. Everything needed for the task exists within the function + attached threadpool
	void DoThreadTasks() {
		for (;;) {
			//If a worker completes some work - they can check here to see if they need to continue again or break out
			if (threadPoolStopped && workerQueue.empty()) {
				return;
			}

			IThreadPool<ReturnType>::worker_type worker;
			{
				//Lock the Queue here!
				unique_lock<mutex> threadQueue(queueLock);

				//If the thread will wait, "wait" will release the lock and proceed onward so we don't block other threads - this threads execution has been blocked
				queueNotify.wait(threadQueue, [this]() { return threadPoolStopped || !workerQueue.empty(); });

				//When it gets woken up - it will verify again that the queue is not empty (verify that it isn't threadPool stopped that notified everyone)
				if (!workerQueue.empty()) {
					worker = std::move(workerQueue.front());
					workerQueue.pop();
				}
			}

			//If we have retrieved a worker - do the work and then start again!
			if (worker != nullptr) {
				ReturnType gRep = worker->doWork();
				{
					lock_guard<mutex> resultLock(resultsLock);
					threadResults.emplace_back(gRep);
				}
			}
		}//For(;;)
	}

};

#endif