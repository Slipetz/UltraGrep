#ifndef __IWORKER__
#define __IWORKER__

#include "GrepResults.h"

//IWorker - Interface for the "worker" threads - Command-type object to run as the "callback" on the threads
class IWorker {
public:
	//DoWork
	//Purpose: Method that must be overridden in the Concrete class - This is the "callback" method
	//		   that gets invoked within the thread. Allows for "Command" pattern-esque functionality
	virtual GrepResults doWork() = 0;
	virtual ~IWorker() {}
};

#endif