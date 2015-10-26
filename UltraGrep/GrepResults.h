#ifndef __GREPRESULTS__
#define __GREPRESULTS__

#include <string>
#include <map>
using namespace std;

class GrepResults {
private:
	//Data Members
	string							fileName;
	map<unsigned long long, string>	matches;

public:
	//Constructor
	GrepResults(string const& fName) : fileName(fName) {}

public:
	void setMatch(unsigned long long const& key, string const& match) {
		matches[key] = match;
	}

	unsigned long long getTotalMatches() const { return matches.size(); }
	string getFileName() const { return fileName; }
	map<unsigned long long, string> getMatches() const { return matches; }

	bool operator <(GrepResults const& rhs) const {
		if (this->fileName < rhs.fileName) {
			return true;
		}
		return false;
	}

};


#endif