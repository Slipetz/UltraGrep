#ifndef __ITHREADPOOL__
#define __ITHREADPOOL__

#include <memory>
#include <vector>
using namespace std;

#include "IWorker.h"
#include "GrepResults.h"

//IThreadPool - Interface for the ThreadPool Objects - Has a few needed functions that get called from external sources for whatever
//			  - threadpool gets instantiated. Also provides a typedef of what type of "Worker" the Pool can expect to use
template <typename ReturnType>
class IThreadPool {
public:
	//Typedef
	using worker_type = unique_ptr<IWorker>;

	virtual void EnqueueWorker(worker_type& worker) = 0;
	virtual vector<ReturnType> getResults() = 0;

	virtual ~IThreadPool() {}
};

#endif