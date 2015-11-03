#include "DirectoryWorker.h"

GrepResults DirectoryWorker::doWork()
{
	//Iterate over the current directory given - generate new "workers" as we go!
	//Blank directory iterator = "end" iterator
	directory_iterator end;
	for (directory_iterator bdi(currentDirectory); bdi != end; ++bdi) {
		path searchedFile = bdi->path();
		if (is_directory(searchedFile)) {
			//Generate new "directory worker" - add it to the queue
			workerFactory.GenerateWorker(WorkerType::DIRECTORY, searchedFile);
		}
		else if (is_regular_file(searchedFile)) {
			//Need to confirm that the "extension" of the file we are searching matches on in our list
			if (isValidExtension(searchedFile.extension().generic_string())) {
				//Generate new "FileWorker" - add it to the queue
				//Only want to track files we're actually scanning
				workerFactory.GenerateWorker(WorkerType::REGULARFILE, searchedFile);
			}
		}
	}

	//Currently - Return empty object as we don't care what this returns
	return GrepResults("");
}

bool DirectoryWorker::isValidExtension(string const & extension)
{
	for (string ext : fileVariables.getAllowedExtensions()) {
		if (ext.compare(extension) == 0) {
			return true;
		}
	}
	return false;
}
