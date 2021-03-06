#ifndef __WORKERFACTORY__
#define __WORKERFACTORY__
#include <filesystem>
using namespace std::tr2::sys;

#include "../UGLib/IThreadPool.h"
#include "C11Logger.h"
#include "../UGLib/FileVariables.h"
#include "../UGLib/FileEnums.h"

//WorkerFactory - Class that generates the IWorker objects and passes them into the ThreadPool Queue
class WorkerFactory {
public:
	//ThreadPool - Interface object that allows us to use virtualization to pass objects to whatever ThreadPool has been created
	IThreadPool<GrepResults>*		threadPool;

	//Blueprint - FileVariables object that contains all of the necessary data for the IWorker objects to perform their tasks
	FileVariables		blueprint;
	
	//WorkerReporter - Provides locked output for the FileDirectory verbose logging. Needs to be in an upper class so all threads see the same logger
	C11Logger			workerReporter;


public:
	//2 Arg Constructor - Creates the object with the ThreadPool pointer and the "blueprint" file that will be passed to each object created
	//					- via the factory
	WorkerFactory(IThreadPool<GrepResults>* tp, FileVariables const& bp) : threadPool(tp), blueprint(bp) {}

	//GenerateWorker
	//Purpose: Generates a new IWorker object based on the given WorkerType and puts it into the ThreadPools queue
	//Accepts: WorkerType - Type of object that the factory needs to create. Path - What file/directory will be parsed/searched by the Worker
	//Returns: Nothing. The IWorker object is passed directly into the ThreadPool
	void GenerateWorker(WorkerType const& fileType, path const& file);
};

#endif