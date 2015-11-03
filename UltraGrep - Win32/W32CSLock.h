#ifndef __W32CSLOCK__
#define __W32CSLOCK__
#include <Windows.h>

//CritSection - OOP-based mutex that utilizes the "Critical Section" Win32 lock mechanism
//			  - Requires an actual "lock" object to invoke the locking of the mechanism (functions currently as a mutex)
class CritSection {
public:
	CRITICAL_SECTION g_LockObj;
public:
	CritSection() { InitializeCriticalSection(&g_LockObj); }
	~CritSection() { DeleteCriticalSection(&g_LockObj); }

private:
	void Lock() { EnterCriticalSection(&g_LockObj); }
	void Unlock() { LeaveCriticalSection(&g_LockObj); }

	friend class W32Lock;
};

//W32Lock - OOP-Based lock that takes a mutex and will invoke the "lock" call on it.
//		  - Scope-based - the lock will be released when the object goes out of scope
class W32Lock {
	CritSection& cs_Lock;
public:
	W32Lock(CritSection& csr) : cs_Lock(csr) { cs_Lock.Lock(); }
	~W32Lock() { cs_Lock.Unlock(); }
};

#endif