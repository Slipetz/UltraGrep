#ifndef __FILEENUMS__
#define __FILEENUMS__

//WorkerType - Enum Class - Allows for easier parsing of what type of "path" file we are working with
//			 - so that the factory can have an easier time generating the correct worker
enum class WorkerType
{
	DIRECTORY,
	REGULARFILE
};

#endif