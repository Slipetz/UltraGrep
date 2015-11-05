#include <string>
#include "C11Logger.h"

void C11Logger::LogOutput(ostream & out, string const & output)
{
	lock_guard<mutex> lock(consoleLock);
	out << output << endl;
}
