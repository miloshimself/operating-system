#include "semaphor.h"
#include "ksemaphr.h"

#include "system.h"
#include <iostream.h>

Semaphore::Semaphore(int init)
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->semInitValue = init;
	(&sysCallInfo)->callType = SEMAPHORE_CREATE;
	(&sysCallInfo)->caller = (void*)this;
	unsigned off = FP_OFF((&sysCallInfo));
	unsigned segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}
	SYSCALLINT;
#endif
	myImpl = (&sysCallInfo)->id;
}

Semaphore::~Semaphore()
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = SEMAPHORE_DESTRUCT;
	(&sysCallInfo)->id = myImpl;
	(&sysCallInfo)->caller = (void*)this;
	unsigned off = FP_OFF((&sysCallInfo));
	unsigned segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}
	SYSCALLINT;
#endif
}

int Semaphore::wait(Time maxTimeToWait)
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	sysCallInfo.callType = SEMAPHORE_WAIT;
	sysCallInfo.id = myImpl;
	sysCallInfo.semWaitTime = maxTimeToWait;
	sysCallInfo.caller = (void*)this;
	unsigned off = FP_OFF((&sysCallInfo));
	unsigned segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}
	SYSCALLINT;
#endif
}

int Semaphore::signal(int n /* = 0 */)
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	sysCallInfo.callType = SEMAPHORE_SIGNAL;
	sysCallInfo.id = myImpl;
	sysCallInfo.semSignalValue = n;
	sysCallInfo.caller = (void*)this;
	unsigned off = FP_OFF((&sysCallInfo));
	unsigned segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}
	SYSCALLINT;
#endif
}

int Semaphore::val() const
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = SEMAPHORE_VALUE;
	(&sysCallInfo)->id = myImpl;
	(&sysCallInfo)->caller = (void*)this;
	unsigned off = FP_OFF((&sysCallInfo));
	unsigned segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}
	SYSCALLINT;
#endif
	intU2 val = (&sysCallInfo)->semValue;
	return val;
}
