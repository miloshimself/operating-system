#include "event.h"


Event::Event(IVTNo ivtNo){
	SystemCallInfo sysCallInfo = SystemCallInfo();
	sysCallInfo.ivtNo = ivtNo;
	sysCallInfo.callType = EVENT_CREATE;
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
	myImpl = sysCallInfo.id;
}

Event::~Event(){
	SystemCallInfo sysCallInfo = SystemCallInfo();
	sysCallInfo.id = myImpl;
	sysCallInfo.callType = EVENT_DESTRUCT;
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

void Event::wait(){
	SystemCallInfo sysCallInfo = SystemCallInfo();
	sysCallInfo.id = myImpl;
	sysCallInfo.callType = EVENT_WAIT;
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

void Event::signal(){
	SystemCallInfo sysCallInfo = SystemCallInfo();
	sysCallInfo.id = myImpl;
	sysCallInfo.callType = EVENT_SIGNAL;
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
