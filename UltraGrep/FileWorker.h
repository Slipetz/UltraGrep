#ifndef __FILEWORKER__
#define __FILEWORKER__
#include <filesystem>
using namespace std::tr2::sys;

#include "IWorker.h"
#include "C11Logger.h"
#include "FileVariables.h"

//FileWorker - Worker Class that will parse a file to look for the given regex within the code file
//			 - Concrete version of an IWorker interface
class FileWorker : public IWorker {
private:
	//Data Members
	FileVariables		fileVariables;
	path				filePath;
	C11Logger&			reportingPipe;
	

public:
	//Constructors - Needs to know the path/file it will be parsing and the fileVariables "blueprint"
	//			   - to determine the regex that will be used to search
	FileWorker(path const& path, FileVariables const& fv, C11Logger& log) : filePath(path), fileVariables(fv), reportingPipe(log) { }

public:
	//DoWork - Overridden Interface Method - PDL is in Interface header
	GrepResults doWork() override;
};

#endif