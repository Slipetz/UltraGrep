#include <iostream>
#include <locale>
#include <algorithm>
using namespace std;

#include "ThreadPool.h"
#include "W32ThreadPool.h"
#include "WorkerFactory.h"
#include "CommandArgumentValidator.h"

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
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	CommandArgumentValidator validator(argc, argv);
	if (validator.HasValidationErrors()) {
		//Error message already printed to cerr from validator
		return EXIT_FAILURE;
	}

	ValidationPackage blueprints = validator.getValidationPackage();
	vector<GrepResults> results;
	LARGE_INTEGER tp11Start, tp11Stop;
	QueryPerformanceCounter(&tp11Start);

	{
		shared_ptr<IThreadPool> threadPool(new ThreadPool());
		WorkerFactory factory(threadPool.get(), blueprints.fileVariables);

		//Start up the thread with what we passed in!
		factory.GenerateWorker(blueprints.initialThreadType, blueprints.initialFile);
		results = threadPool->getResults();
	}

	sort(results.begin(), results.end());
	QueryPerformanceCounter(&tp11Stop);
	double tp11Elapsed = (tp11Stop.QuadPart - tp11Start.QuadPart) / double(frequency.QuadPart);
	cout << "C++11 Completed and sorted in " << tp11Elapsed << endl;

	LARGE_INTEGER w32tpstart, w32tpstop;
	QueryPerformanceCounter(&w32tpstart);

	{
		shared_ptr<IThreadPool> threadPool(new W32ThreadPool());
		WorkerFactory factory(threadPool.get(), blueprints.fileVariables);

		//Start up the thread with what we passed in!
		factory.GenerateWorker(blueprints.initialThreadType, blueprints.initialFile);
		results = threadPool->getResults();
	}

	sort(results.begin(), results.end());
	QueryPerformanceCounter(&w32tpstop);
	double w32ElapsedTime = (w32tpstop.QuadPart - w32tpstart.QuadPart) / double(frequency.QuadPart);
	cout << "W32 Completed and sorted in " << w32ElapsedTime << endl;


	//OutputReport(results);
}


