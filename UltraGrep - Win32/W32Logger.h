#ifndef __W32LOGGER__
#define __W32LOGGER__

#include "ILogger.h"
#include "W32CSLock.h"

class W32Logger : ILogger {
	CritSection lk_Console;

public:
	void LogOutput(ostream&, string const&) override;
};

#endif