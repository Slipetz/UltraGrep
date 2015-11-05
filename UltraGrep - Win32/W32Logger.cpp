#include <string>
using namespace std;
#include "W32Logger.h"

void W32Logger::LogOutput(ostream & output, string const & loggedLine)
{
	W32Lock lock(lk_Console);
	output << loggedLine << endl;
}
