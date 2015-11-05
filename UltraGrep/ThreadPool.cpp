#include <iostream>
using namespace std;
#include "ThreadPool.h"

void ThreadPool::EnqueueWorker(worker_type& worker)
{
	lock_guard<mutex> queueGuard(queueLock);
	workerQueue.push(std::move(worker));
	queueNotify.notify_one();
}


void ThreadPool::DoThreadTasks()
{
	for (;;) {
		//If a worker completes some work - they can check here to see if they need to continue again or break out
		if (threadPoolStopped && workerQueue.empty()) {
			return;
		}

		worker_type worker;
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
			GrepResults gRep = worker->doWork();
			if (gRep.getTotalMatches() > 0) {
				{
					lock_guard<mutex> resultLock(resultsLock);
					threadResults.emplace_back(gRep);
				}
			}
		}
	}//For(;;)
}


vector<GrepResults> ThreadPool::getResults()
{
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

ThreadPool::ThreadPool() : threadPoolStopped(false)
{
	cout << "Using C++11 ThreadPool..." << endl;
	size_t numOfThreads = thread::hardware_concurrency();
	for (size_t i = 0; i < numOfThreads; ++i) {
		threadHolder.emplace_back(thread(&ThreadPool::DoThreadTasks, this));
	}
}

ThreadPool::ThreadPool(size_t const & poolSize) : threadPoolStopped(false) {
	cout << "Using C++11 ThreadPool..." << endl;
	for (size_t i = 0; i < poolSize; ++i) {
		threadHolder.emplace_back(thread(&ThreadPool::DoThreadTasks, this));
	}
}

ThreadPool::~ThreadPool()
{
	threadPoolStopped = true;
	queueNotify.notify_all();
	for (auto& ft : threadHolder) {
		ft.join();
	}
}
