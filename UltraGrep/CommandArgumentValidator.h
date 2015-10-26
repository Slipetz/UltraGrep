#ifndef __CMD_ARG_VALID__
#define __CMD_ARG_VALID__

#include <iostream>
using namespace std;

#include "ValidationPackage.h"

class CommandArgumentValidator {
	
	//Data Members - Only needs the validation package. Allows us to get what we need out of the command args!
	ValidationPackage		valPackage;
	string					properUsage;
	bool					anyInvalidArgument;

public:
	CommandArgumentValidator(int argc, char* argv[]);

private:
	bool IsInvalidPath(string filePath);
	vector<string> ParseExtensions(string extString);

public:
	bool HasValidationErrors() { return anyInvalidArgument; }
	ValidationPackage getValidationPackage() { return valPackage; }
};

#endif