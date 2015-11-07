#ifndef __CMD_ARG_VALID__
#define __CMD_ARG_VALID__
#include "ValidationPackage.h"

//CommandArgumentValidator - Object that will take the Command Arguments passed in and parse them into a package
//						   - that can be retrieved to be passed to other objects as needed
class CommandArgumentValidator {
	
	//Validation Package - Items that are parsed from the Command Arguments to be passed to the objects that need to
	//					 - know about the input arguments
	ValidationPackage		valPackage;

	//AnyInvalidArgument - Bool that is set to true if any of the validation fails in a way to stop the program
	bool					anyInvalidArgument;

	//ValidRgx - Class variable that gets assigned if the given regex input is valid. If not, it will output a message and set anyInvalidArgument to true to cause validation error
	regex					validRgx;

public:

	//Constructor - Takes the command arguments and validates them upon object construction
	CommandArgumentValidator(int argc, char* argv[]);

private:
	//IsInvalidPath 
	//Purpose: Checks to see if the "file" argument from the Command Args is indeed a valid path
	//Accepts - String that contains the "file" argument from the Command Args
	//Returns - True if the path is invalid, else it will return false
	bool IsInvalidPath(string filePath);

	//ParseExtensions
	//Purpose: Parses the string containing all of the extensions passed into the Command Args and puts them into a vector.
	//		   If the string doesn't start with a period, it WILL output a message and default to .txt
	//Accepts: String containing the extensions passed in the command arguments.
	vector<string> ParseExtensions(string extString);

	void GenerateRegex(string const& rgx);

public:
	//HasValidationError - Getter for the anyInvalidArgument - Allows us to see if any validation errors came up
	bool HasValidationErrors() { return anyInvalidArgument; }

	//GetValidationPackage - Allows us to retrieve the ValidationPackage that contains the parsed command arguments
	ValidationPackage getValidationPackage() { return valPackage; }
};

#endif