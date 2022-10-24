#ifndef H_PCB_H_
#define H_PCB_H_

#include "basetype.h"
#include "sysobj.h"

class Thread;
class KernelSem;

class PCB : public SystemObject
{
public:
	friend class System;
	friend class KernelSem;
	friend void interrupt systemCall(...);
	friend void interrupt timer(...);

	void initPCB();
	void initKernelPCB();
	void initIdlePCB();

	static void idleWrapper();

	void start();

	void waitToComplete();

	void resetTimeSlice();

	BOOLEAN started;
	BOOLEAN completed;

	PCB(StackSize _stackSize, Time _timeSlice, Thread* _owner);
	~PCB();

	intU2* stack;
	StackSize stackSize;
	REG bp, sp, ss;
	Time timeSlice;
	Time remaining;
	Time timeToWaitOnSem;
	volatile BOOLEAN unblockedBeforeTimeElapsed;  // TRUE ako je nit deblokirana zbog isteka vremena, FALSE u ostalim slucajevima
	STATE state;
	Thread* owner;

	KernelSem* sem;

	static ID NumOfThreads;
};


#endif /* H_PCB_H_ */
