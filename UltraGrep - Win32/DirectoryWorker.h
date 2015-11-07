#ifndef __DIRWORKER__
#define __DIRWORKER__
#include <filesystem>
using namespace std::tr2::sys;

#include "../UGLib/IWorker.h"
#include "../UGLib/FileVariables.h"
#include "WorkerFactory.h"

//FileWorker - Worker Class that will scan a directory and create an IWorker object for every directory or every file
//			 - that matches the given extensions. NOT recursive - only scans the directory thats given to it
class DirectoryWorker : public IWorker {
private:
	//WorkerFactory - Reference to the WorkerFactory object. Needed to create new objects found in it's search
	WorkerFactory&		workerFactory;

	//FileVariables - Holds the Command Argument data that was parsed - needed to determine what extensions are valid
	FileVariables		fileVariables;

	//CurrentDirectory - Path file that represents the directory that is to be scanned in the doWork function
	path				currentDirectory;

public:
	//3-Arg Constructor - Provides the DirectoryWorker object will everything it needs to parse current directory and create new worker objects via the factory
	DirectoryWorker(path const& dir, FileVariables const& fv, WorkerFactory& factory) : currentDirectory(dir), fileVariables(fv), workerFactory(factory) { }

public:
	//DoWork - Concrete version of the base function. PDL in IWorker interface
	GrepResults doWork() override;
	
private:
	//IsValidExtension
	//Purpose: Determines if the extension gathered from the path file contains an extension that was passed in via the command line
	//Accepts: String containing the extension from the path object
	//Returns: Bool - true if the extension matches one found in the extensions vectory from the File Variables
	bool isValidExtension(string const& extension);

};

#endif