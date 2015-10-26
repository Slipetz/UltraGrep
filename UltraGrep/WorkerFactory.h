#ifndef __WORKERFACTORY__
#define __WORKERFACTORY__

#include <iostream>
using namespace std;
#include <filesystem>
using namespace std::tr2::sys;

#include "ThreadPool.h"
#include "FileVariables.h"
#include "FileEnums.h"

class WorkerFactory {
public:
	//Needs a reference to the ThreadPool - Need him to add to the ThreadPool Queue
	ThreadPool&			threadPool;
	FileVariables		blueprint;

public:
	WorkerFactory(ThreadPool& tp, FileVariables const& bp) : threadPool(tp), blueprint(bp) {}
	void GenerateWorker(ThreadType const& fileType, path const& file);

};

#endif