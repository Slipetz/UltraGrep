#ifndef __VALIDATIONPCKG__
#define __VALIDATIONPCKG__

#include <filesystem>
using namespace std::tr2::sys;

#include "FileVariables.h"
#include "FileEnums.h"

//ValidationPackage - Simple holder class that is generated and populated in the CommandArgValidator class
class ValidationPackage {
public:
	//Data Members - Worker Blueprint
	FileVariables		fileVariables;
	path				initialFile;
	WorkerType			initialThreadType;
};

#endif