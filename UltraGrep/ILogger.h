#ifndef __ILOGGER__
#define __ILOGGER__

#include <iostream>
using namespace std;

class ILogger {
public:

	virtual void LogOutput(ostream&, string const&) = 0;
};

#endif