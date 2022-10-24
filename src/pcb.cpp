#include "pcb.h"

#include <dos.h>
#include "thread.h"
#include "system.h"
#include "vector.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "ksemaphr.h"


PCB::PCB(StackSize _stackSize, Time _timeSlice, Thread* _owner)
{
	stackSize = _stackSize;
	timeSlice = _timeSlice;
	owner = _owner;

	stack = NULL;
	sem =  new KernelSem(0);
	started = FALSE;
	completed = FALSE;
	remaining = 0;
	timeToWaitOnSem = 0;
	state = START;

}

PCB::~PCB()
{
	delete[] stack;
	delete sem;
}

void PCB::initPCB()
{
	stack = new intU2[stackSize];

	stack[stackSize - 1] = 0x200; // PSW I flag
	stack[stackSize - 2] = FP_SEG(Thread::wrapper);
	stack[stackSize - 3] = FP_OFF(Thread::wrapper);

	sp = FP_OFF(stack + stackSize -12);
	ss = FP_SEG(stack + stackSize -12);
	bp = sp;

	state = READY;
	completed = FALSE;
}

void PCB::initKernelPCB()
{
	stack = new intU2[stackSize];

	sp = FP_OFF(stack + stackSize);
	ss = FP_SEG(stack + stackSize);
	bp = FP_OFF(stack + stackSize);

	state = KERNEL;
}

void PCB::initIdlePCB()
{
	stack = new intU2[stackSize];

	stack[stackSize-1] = 0x200;
	stack[stackSize-2] = FP_SEG(idleWrapper);
	stack[stackSize-3] = FP_OFF(idleWrapper);

	sp = FP_OFF(stack + stackSize-12);
	ss = FP_SEG(stack + stackSize-12);
	bp = sp;

	state = IDLE;
}

void PCB::start()
{
	if( ! started)
	{
		initPCB();
		started = TRUE;
		state = READY;
		System::lockFlag = TRUE;
		cout << "Dodavanje PCB-a sa IDem " << System::vectorPCB->getID((PCB*)this) << " u Scheduler." << endl;
		System::lockFlag = FALSE;
		Scheduler::put((PCB*)this);
	}
}

void PCB::idleWrapper()
{
	while(1)
	{
		for (int i =0; i < 30; ++i)
		{
			for (int k = 0; k<10000; ++k)
				for (int j = 0; j <30000; ++j);
		}
	}
}

void PCB::waitToComplete()
{
	if(completed   ||   ! started)
	{
		return;
	}

	sem->wait(0);
}

void PCB::resetTimeSlice()
{
	remaining = timeSlice;
}
