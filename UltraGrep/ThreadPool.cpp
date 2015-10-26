#include <iostream>
using namespace std;
#include "ThreadPool.h"

void ThreadPool::EnqueueWorker(worker_type& worker)
{
	lock_guard<mutex> queueGuard(queueLock);
	workerQueue.push(std::move(worker));
	queueNotify.notify_one();
}


void ThreadPool::AssignWorker()
{
	for (;;) {
		worker_type worker;
		{
			unique_lock<mutex> threadQueue(queueLock);
			queueNotify.wait(threadQueue, [this]() { return threadPoolStopped || !workerQueue.empty(); });
			if (threadPoolStopped && workerQueue.empty()) {
				return;
			}

			if (!workerQueue.empty()) {
				worker = std::move(workerQueue.front());
				workerQueue.pop();
			}
		}
		if (worker != nullptr) {
			GrepResults gRep = worker->doWork();
			if (gRep.getTotalMatches() > 0) {
				{
					lock_guard<mutex> resultLock(resultsLock);
					threadResults.emplace_back(gRep);
				}
			}
		}
	}
}

void ThreadPool::StopPool()
{
	threadPoolStopped = true;
	queueNotify.notify_all();
	for (auto& ft : futuresThread) {
		ft.get();
	}
	//Kill the futuresThread - once they've been used, they're done!
	futuresThread.clear();
}

vector<GrepResults> ThreadPool::getResults()
{
	{
		lock_guard<mutex> resultsGuard(resultsLock);
		return threadResults;
	}
}

ThreadPool::ThreadPool() : threadPoolStopped(false)
{
	numOfThreads = thread::hardware_concurrency();
	for (size_t i = 0; i < numOfThreads; ++i) {
		future<void> tFuture = async(launch::async, &ThreadPool::AssignWorker, this);
		futuresThread.emplace_back(std::move(tFuture));
	}
}

ThreadPool::ThreadPool(size_t const & poolSize) : threadPoolStopped(false) {
	for (size_t i = 0; i < poolSize; ++i) {
		future<void> tFuture = async(launch::async, &ThreadPool::AssignWorker, this);
		futuresThread.emplace_back(std::move(tFuture));
	}
}

ThreadPool::~ThreadPool()
{
	threadPoolStopped = true;
	queueNotify.notify_all();
	for (auto& ft : futuresThread) {
		ft.get();
	}
}
