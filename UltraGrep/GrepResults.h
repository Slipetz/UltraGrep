#ifndef __GREPRESULTS__
#define __GREPRESULTS__

#include <string>
#include <map>
using namespace std;

//GrepResults - Result object that each "worker" will pass back to the Pool to be collected after
class GrepResults {
private:
	//FileName - The name of the file that was parsed - Used in the sort to determine what order the data needs to be displayed
	string							fileName;

	//Matches - Map that corresponds the "line number" to the match on that line for that given file
	map<unsigned long long, string>	matches;

public:
	//Constructor
	GrepResults(string const& fName) : fileName(fName) {}

public:
	//SetMatch
	//Purpose: Sets the match to the given key in the Matches Map (Key = LineNumber, Value = Matched string)
	//Accepts - Unsigned Long Long Key - Represents the linenumber match was found on. String Match - String that was found to be a match
	//Returns - Nothing - Data is populated directly to the map in this function
	void setMatch(unsigned long long const& key, string const& match) {
		matches[key] = match;
	}

	//GetTotalMatches - Returns the number of matches found in the particular file
	unsigned long long getTotalMatches() const { return matches.size(); }

	//GetFileName - Returns the filename of this Results Object
	string getFileName() const { return fileName; }

	//GetMatches - Returns the map - used in the Reporting of the data (allows us to see the matches and their line numbers)
	map<unsigned long long, string> getMatches() const { return matches; }

	//Operator <
	//Purpose: Override the operator so that these objects can be used in the sort() function of a vector/STL container
	//Accepts: Another GrepResults object to compare against
	//Returns: Bool - True if the current object's filename is already sorted (in DESC order for this function)
	bool operator <(GrepResults const& rhs) const {
		if (this->fileName < rhs.fileName) {
			return true;
		}
		return false;
	}

};
#endif