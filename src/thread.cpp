#include "thread.h"

#include "pcb.h"
#include "system.h"
#include <iostream.h>
#include "vector.h"



Thread::Thread(StackSize stackSize, Time timeSlice)
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = THREAD_CREATE;
	(&sysCallInfo)->stackSize = stackSize;
	(&sysCallInfo)->timeSlice = timeSlice;
	(&sysCallInfo)->caller = (void*)this;

	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif

	myPCB = sysCallInfo.id;
}

Thread::~Thread()
{
	waitToComplete();

	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = THREAD_DESTRUCT;
	(&sysCallInfo)->caller = (void*)this;
	(&sysCallInfo)->id = myPCB;

	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif
}

void Thread::run()
{}

void Thread::start()
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = THREAD_START;
	(&sysCallInfo)->caller = (void*)this;
	(&sysCallInfo)->id = myPCB;

	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif
}

void Thread::waitToComplete()
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = THREAD_WAITTOCOMPLETE;
	(&sysCallInfo)->caller = (void*)this;
	(&sysCallInfo)->id = myPCB;

	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif

}

ID Thread::getId()
{
	return myPCB;
}

ID Thread::getRunningId()
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = THREAD_GETRUNNINGID;

	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif

	return sysCallInfo.runningID;
}

Thread* Thread::getThreadById(ID id)
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = THREAD_GETTHREADBYID;
	(&sysCallInfo)->id = id;

	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif

	return (Thread*)sysCallInfo.threadByID;
}

void dispatch()
{
	System::dispatch();
}

void Thread::wrapper()
{
	((PCB*)System::running)->owner->run();

	SystemCallInfo sysCallInfo;
	sysCallInfo.callType = THREAD_FINISH;
	sysCallInfo.caller = (void*)System::running;
	sysCallInfo.id = ((PCB*)System::running)->owner->myPCB;
	intU2 off = FP_OFF((&sysCallInfo));
	intU2 segg = FP_SEG((&sysCallInfo));
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov cx, segg
		mov dx, off
	}

	SYSCALLINT;
#endif
}
