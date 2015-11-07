#ifndef __FILEWORKER__
#define __FILEWORKER__
#include <filesystem>
using namespace std::tr2::sys;

#include "../UGLib/IWorker.h"
#include "../UGLib/FileVariables.h"
#include "W32Logger.h"

//FileWorker - Worker Class that will parse a file to look for the given regex within the code file
//			 - Concrete version of an IWorker interface
class FileWorker : public IWorker {
private:
	//Data Members
	FileVariables		fileVariables;
	path				filePath;
	W32Logger&			reportingPipeline;

public:
	//Constructors - Needs to know the path/file it will be parsing and the fileVariables "blueprint"
	//			   - to determine the regex that will be used to search
	FileWorker(path const& path, FileVariables const& fv, W32Logger& pipe) : filePath(path), fileVariables(fv), reportingPipeline(pipe) { }

public:
	//DoWork - Overridden Interface Method - PDL is in Interface header
	GrepResults doWork() override;
};

#endif