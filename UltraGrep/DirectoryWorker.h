#ifndef __DIRWORKER__
#define __DIRWORKER__

#include <string>
using namespace std;
#include <filesystem>
using namespace std::tr2::sys;

#include "IWorker.h"
#include "FileVariables.h"
#include "WorkerFactory.h"

class DirectoryWorker : public IWorker {
public:
	//Data Members
	WorkerFactory&		workerFactory;
	FileVariables		fileVariables;
	path				currentDirectory;

public:
	//Constructor
	DirectoryWorker(path const& dir, FileVariables const& fv, WorkerFactory& factory) : currentDirectory(dir), fileVariables(fv), workerFactory(factory) { }

public:
	GrepResults doWork() override;
	
private:
	bool isValidExtension(string const& extension);

};

#endif