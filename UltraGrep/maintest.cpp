#include <iostream>
#include <locale>
#include <algorithm>
using namespace std;


#include "ThreadPool.h"
#include "WorkerFactory.h"
#include "ValidationPackage.h"
#include "CommandArgumentValidator.h"

void OutputReport(vector<GrepResults>& results) {
	sort(results.begin(), results.end());
	cout << "Grep Results: " << endl;
	unsigned long long numMatches = 0;
	for (auto& result : results) {
		cout << result.getFileName() << endl;
		cout << string(result.getFileName().length(), '*') << endl;
		for (auto& kvp : result.getMatches()) {
			cout << "[" << kvp.first << "] " << kvp.second << endl;
		}
		numMatches += result.getTotalMatches();
		cout << endl;
	}
	cout << "Files with Matches: " << results.size() << endl;
	cout << "Matches Found: " << numMatches << endl;
}


int main(int argc, char* argv[]) {

	cout.imbue(locale(""));

	CommandArgumentValidator validator(argc, argv);
	if (validator.HasValidationErrors()) {
		return EXIT_FAILURE;
	}

	ValidationPackage blueprints = validator.getValidationPackage();

	ThreadPool threadPool;
	WorkerFactory factory(threadPool, blueprints.fileVariables);

	//Start up the thread with what we passed in!
	factory.GenerateWorker(blueprints.initialThreadType, blueprints.initialFile);

	//Get the Results
	#if defined _DEBUG
		Sleep(500); //Need to give the first thread time to spawn some tasks before we exit it. Thanks Debug!
	#endif
	threadPool.StopPool();
	vector<GrepResults> results = threadPool.getResults();
	OutputReport(results);
}


