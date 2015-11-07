#include <iostream>
#include <locale>
#include <algorithm>
using namespace std;

#include "W32ThreadPool.h"
#include "WorkerFactory.h"
#include "../UGLib/CommandArgumentValidator.h"

void OutputReport(vector<GrepResults> const& results) {
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
		//Error message already printed to cerr from validator
		return EXIT_FAILURE;
	}

	ValidationPackage blueprints = validator.getValidationPackage();
	vector<GrepResults> results;
	
	{
		shared_ptr<IThreadPool<GrepResults>> threadPool(new W32ThreadPool<GrepResults>());
		WorkerFactory factory(threadPool.get(), blueprints.fileVariables);

		//Start up the thread with what we passed in!
		factory.GenerateWorker(blueprints.initialThreadType, blueprints.initialFile);
		results = threadPool->getResults();
	}
	
	//Don't like this, but to make the Threadpool more generic, this has to be added here
	results.erase(remove_if(results.begin(), results.end(), [] (GrepResults res) {
		return res.getTotalMatches() == 0;
	}), results.end());

	sort(results.begin(), results.end());
	OutputReport(results);
}


