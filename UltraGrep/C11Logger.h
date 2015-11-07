#ifndef __C11LOGGER__
#define __C11LOGGER__

#include <iostream>
#include <mutex>
using namespace std;

#include "../UGLib/ILogger.h"

class C11Logger : public ILogger {
private:
	mutex		consoleLock;
public:
	void LogOutput(ostream&, string const&) override;
};
#endif