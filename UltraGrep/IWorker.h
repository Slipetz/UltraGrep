#ifndef __IWORKER__
#define __IWORKER__

#include "GrepResults.h"

//IWorker - Interface for the "worker" threads - Command-type object to run the threads
class IWorker {
public:
	virtual GrepResults doWork() = 0;
	virtual ~IWorker() {}
};

#endif