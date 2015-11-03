#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#include "FileWorker.h"

GrepResults FileWorker::doWork() {
	GrepResults results(filePath.generic_string());

	//First - need to open the file
	ifstream fileToParse(filePath);

	if (fileToParse.fail()) {
		cout << "File: " << filePath.generic_string() << " was deleted during execution! Abandoning parse!" << endl;
		return results;
	}

	if (fileVariables.isVerbose()) {
		cout << "Grepping: " << filePath.generic_string() << endl;
	}
	//Once file is open -> read through the file and check to see if we can find the regex in the line
	string lineToParse;
	unsigned long long lineNumber = 1;
	while (getline(fileToParse, lineToParse)) {
		if (regex_search(lineToParse, fileVariables.getRegexString())) {
			if (fileVariables.isVerbose()) {
				cout << "Matched: [" << lineNumber << "]: " << lineToParse << endl;
			}
			//Means we have a match. Attach it to the result object!
			results.setMatch(lineNumber, lineToParse);
		}
		++lineNumber;
	}

	return results;
}