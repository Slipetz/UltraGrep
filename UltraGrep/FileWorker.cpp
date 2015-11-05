#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
using namespace std;
#include "FileWorker.h"

GrepResults FileWorker::doWork() {
	GrepResults results(filePath.generic_string());

	//First - need to open the file
	ifstream fileToParse(filePath);

	if (fileToParse.fail()) {
		reportingPipe.LogOutput(cout, "File: " + filePath.generic_string() + " was deleted during execution! Abandoning parse!");
		return results;
	}

	if (fileVariables.isVerbose()) {
		reportingPipe.LogOutput(cout, "Grepping: " + filePath.generic_string());
	}
	//Once file is open -> read through the file and check to see if we can find the regex in the line
	string lineToParse;
	unsigned long long lineNumber = 1;
	while (getline(fileToParse, lineToParse)) {
		if (regex_search(lineToParse, fileVariables.getRegexString())) {
			if (fileVariables.isVerbose()) {
				reportingPipe.LogOutput(cout, "Matched: [" + to_string(lineNumber) + "]: " + lineToParse);
			}
			//Means we have a match. Attach it to the result object!
			results.setMatch(lineNumber, lineToParse);
		}
		++lineNumber;
	}

	return results;
}
