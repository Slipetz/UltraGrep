#ifndef __VALIDATIONPCKG__
#define __VALIDATIONPCKG__

#include <iostream>
using namespace std;
#include <filesystem>
using namespace std::tr2::sys;

#include "FileVariables.h"
#include "FileEnums.h"

class ValidationPackage {
public:
	//Data Members - Worker Blueprint
	FileVariables		fileVariables;
	path				initialFile;
	ThreadType			initialThreadType;
};

#endif