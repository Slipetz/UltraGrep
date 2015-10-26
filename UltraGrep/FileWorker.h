#ifndef __FILEWORKER__
#define __FILEWORKER__

#include <string>
#include <vector>
using namespace std;
#include <filesystem>
using namespace std::tr2::sys;

#include "IWorker.h"
#include "FileVariables.h"

//FileWorker - Threaded Class that will parse a file to look for the given regex within the code file
class FileWorker : public IWorker {
public:
	//Data Members
	FileVariables		fileVariables;
	path				filePath;

public:
	//Constructors
	FileWorker(path const& path, FileVariables const& fv) : filePath(path), fileVariables(fv) { }

public:
	GrepResults doWork() override;
};

#endif