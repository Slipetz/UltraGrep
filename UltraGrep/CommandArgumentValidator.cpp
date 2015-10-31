#include <iostream>
using namespace std;
#include "CommandArgumentValidator.h"

CommandArgumentValidator::CommandArgumentValidator(int argc, char * argv[]) : anyInvalidArgument(false)
{
	string programUsage = "UltraGrep [-v] File/Directory Regex [file extensions]\n";
	//Program must have at least 3 parameters (Program + Path + Regex) and cannot have more than 5 (Verbose + Extensions added)
	if (argc < 3 || argc > 5) {
		cerr << "Incorrect number of parameters. Proper usage:\n";
		cerr << programUsage;
		anyInvalidArgument = true;
		return;
	}

	//Argc 3 = No extra arguments!
	if (argc == 3) {
		//Based on Args - 1 should be File/Directory. Check it and confirm that it exists!
		if (IsInvalidPath(argv[1])) {
			anyInvalidArgument = true;
			return;
		}
		valPackage.fileVariables = FileVariables(regex(argv[2]));
	}
	else if (argc == 4) {
		//Means one of the arguments is extra. Need to figure out which one!
		string vbCheck = argv[1];
		if (vbCheck.compare("-v") == 0 || vbCheck.compare("-V") == 0) {
			//Means that the Verbose flag has been set. Set Verbose in the FileVariables below!
			if (IsInvalidPath(argv[2])) { //File/Directory should be the second command arg now
				anyInvalidArgument = true; 
				return;
			}
			valPackage.fileVariables = FileVariables(regex(argv[3]), { ".txt" }, true);
		}
		else {
			//If we hit here, verbose flag wasn't set OR they've got the wrong flag. Should be caught by the "file exists" check in the file check
			if (IsInvalidPath(argv[1])) { //First argument now is the File/Directory
				anyInvalidArgument = true;
				return;
			}
			regex rgx(argv[2]); //Second argument is the regex
			vector<string> parsedExtensions = ParseExtensions(argv[3]);
			valPackage.fileVariables = FileVariables(rgx, parsedExtensions);
		}
	}
	else 
	{
		//1 - We know we have verbose
		string vbCheck = argv[1];
		bool verbose = vbCheck.compare("-v") == 0 || vbCheck.compare("-V") == 0;
		if (!verbose) {
			cerr << "Verbose flag MUST be -v or -V. Verbosity will be turned off";
		}

		//2 - File
		if (IsInvalidPath(argv[2])) {
			anyInvalidArgument = true;
			return;
		}
		//3 - Regex
		regex rgx(argv[3]);
		//4 - File Extensions
		vector<string> parsedExtensions = ParseExtensions(argv[4]);

		valPackage.fileVariables = FileVariables(rgx, parsedExtensions, verbose);
	}
}

bool CommandArgumentValidator::IsInvalidPath(string arg)
{
	path filePath(arg);
	if (!exists(filePath)) {
		cerr << "File: " << arg << " doesn't exist!" << endl;
		cerr << "Program Usage: " << "UltraGrep [-v] File/Directory Regex [file extensions]\n";
		return true;
	}
	else 
	{
		valPackage.initialFile = filePath;
		if (is_directory(filePath)) {
			valPackage.initialThreadType = WorkerType::DIRECTORY;
			return false;
		} 
		valPackage.initialThreadType = WorkerType::REGULARFILE;
		return false;
	}
}

vector<string> CommandArgumentValidator::ParseExtensions(string extString)
{
	//We now that they are "separated" by periods. Check for each period, attach that to the vector!
	if (extString[0] != '.') {
		//Not a valid extension!
		cerr << "Extension string invalid. Ensure string starts with a . and separated by .\nDefaulting to .txt only";
		return{ ".txt" };
	}

	//Need to parse until we get to the second "."
	size_t subStart = 0;
	vector<string> extensions;
	while (subStart < extString.length()) {
		//Increment by one, so we don't retrieve the second period when parsing the string
		size_t length = extString.find_first_of('.', subStart + 1) - subStart;
		if (length == string::npos) {
			extensions.emplace_back(extString.substr(subStart, extString.length()));
			break; //The final parsed string
		}
		else {
			string extension = extString.substr(subStart, length);
			subStart += length;
			extensions.emplace_back(extension);
		}
	}

	return extensions;
}
