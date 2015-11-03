#ifndef __FILEWORKER__
#define __FILEWORKER__
#include <filesystem>
using namespace std::tr2::sys;

#include "IWorker.h"
#include "FileVariables.h"

//FileWorker - Worker Class that will parse a file to look for the given regex within the code file
//			 - Concrete version of an IWorker interface
class FileWorker : public IWorker {
private:
	//Data Members
	FileVariables		fileVariables;
	path				filePath;

public:
	//Constructors - Needs to know the path/file it will be parsing and the fileVariables "blueprint"
	//			   - to determine the regex that will be used to search
	FileWorker(path const& path, FileVariables const& fv) : filePath(path), fileVariables(fv) { }

public:
	//DoWork - Overridden Interface Method - PDL is in Interface header
	GrepResults doWork() override;
};

#endif