#ifndef __FILEVARIABLES__
#define __FILEVARIABLES__

#include <regex>
#include <vector>
using namespace std;

class FileVariables {
private:
	//Data Members - Different variables that a Worker Object will require to do their work
	//Private with only setters - don't want these objects being recreated/to be injected
	regex				rgxMatchedText;
	vector<string>		allowedExtensions;
	bool				verbose;

public:
	FileVariables() {}
	FileVariables(regex const& rgx, vector<string> const& exts = { ".txt" }, bool const& verbose = false) : rgxMatchedText(rgx), allowedExtensions(exts), verbose(verbose) {}

	regex getRegexString() { return rgxMatchedText; }
	vector<string> getAllowedExtensions() { return allowedExtensions; }
	bool isVerbose() { return verbose; }
};

#endif