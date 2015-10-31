#ifndef __FILEVARIABLES__
#define __FILEVARIABLES__

#include <regex>
#include <vector>
using namespace std;

//FilesVariables - Holder class for parsed data - the IWorker objects need this data to properly do their work!
//				 - This allows us to create a "blueprint" that gets passed from the Factory to the worker
class FileVariables {
private:
	//Data Members - Different variables that a Worker Object will require to do their work
	//Private with only setters - don't want these objects being recreated/data to be injected
	regex				rgxMatchedText;
	vector<string>		allowedExtensions;
	bool				verbose;

public:
	//Need the 0-arg constructor as the "CommandArgValidator" needs to create a template one on it's own creation
	FileVariables() {}

	//3 Arg Constructor - Pass the required "blueprint" items to create a full blueprint for the Workers. Only the regex is needed as the other
	//					- arguments are optional for the program and are set to defaults
	FileVariables(regex const& rgx, vector<string> const& exts = { ".txt" }, bool const& verbose = false) : rgxMatchedText(rgx), allowedExtensions(exts), verbose(verbose) {}

	//GetRegexString - Getter for rgxMatchedText
	regex getRegexString() const { return rgxMatchedText; }

	//GetAllowedExtensions - Getter for allowedExtensions
	vector<string> getAllowedExtensions() const { return allowedExtensions; }

	//IsVerbose - Getter for verbose
	bool isVerbose() const { return verbose; }
};

#endif