#include "system.h"

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"

#include "thread.h"
#include "pcb.h"

#include "semaphor.h"
#include "ksemaphr.h"

#include "sysobj.h"
#include "vector.h"

#include "ivtentry.h"
#include "kernelev.h"



const IVTNo System::IVTNo_Timer = 0x08;
const IVTNo System::IVTNo_SystemCall = 0x60;

IntRoutine System::oldTimerInterrupt = timer;
IntRoutine System::oldSystemCallInterrupt = systemCall;

void interrupt timer(...);
void interrupt systemCall(...);

extern void tick();

BOOLEAN System::lockFlag = FALSE;
BOOLEAN System::systemMode = FALSE;
SystemCallInfo* sysCallInfo = 0;

PCB* volatile System::running = 0;
PCB* System::kernel = 0;
PCB* System::idle = 0;

VectorSysObj* System::vectorPCB = 0;
VectorSysObj* System::vectorSem = 0;
VectorSysObj* System::vectorKEvent = 0;
KernelEv* KernelEvList[256];

REG tbp, tsp, tss, toff, tseg;


void System::initSystem()
{
	createMainPCB();
	createIdlePCB();
	createKernelPCB();
	initSystemFiles();

	initTimer();
	initSystemCall();
}

void System::restoreSystem()
{
	deleteSystemFiles();

	IVTEntry::restoreInterrupts();

	restoreSystemCall();
	restoreTimer();

	deleteIdlePCB();
	deleteKernelPCB();
	deleteMainPCB();
}

void System::initSystemFiles()
{
	System::vectorPCB = new VectorSysObj();
	System::vectorSem = new VectorSysObj();
	System::vectorKEvent = new VectorSysObj();
}

void System::deleteSystemFiles()
{
	VECTOR_SIZE lengthPCB = vectorPCB->getLength();

	ID i = 0;
	for(i = 0; i<lengthPCB; i++)
	{
		SystemObject *sysObj = vectorPCB->get(i);
		if(sysObj != NULL)
		{
			((PCB*)sysObj)->waitToComplete();
		}
	}

	for(i = 0; i<lengthPCB; i++)
	{
		delete vectorPCB->remove(i);
	}

	VECTOR_SIZE lengthSemaphore = vectorSem->getLength();
	for(i = 0; i<lengthSemaphore; i++)
	{
		delete vectorSem->remove(i);
	}

	VECTOR_SIZE lengthEvent = vectorKEvent->getLength();
	for(i = 0; i<lengthEvent; i++)
	{
		delete vectorKEvent->remove(i);
	}

	delete vectorPCB;
	delete vectorSem;
	delete vectorKEvent;
}

void System::initTimer()
{
#ifndef BCC_BLOCK_IGNORE
	oldTimerInterrupt = getvect(IVTNo_Timer);
	setvect(IVTNo_Timer, timer);
#endif
}

void System::restoreTimer()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(IVTNo_Timer, oldTimerInterrupt);
#endif
}

void System::initSystemCall()
{
#ifndef BCC_BLOCK_IGNORE
	oldSystemCallInterrupt = getvect(IVTNo_SystemCall);
	setvect(IVTNo_SystemCall, systemCall);
#endif
}

void System::restoreSystemCall()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(IVTNo_SystemCall, oldSystemCallInterrupt);
#endif
}

void System::createMainPCB()
{
	running = new PCB(0, 1, NULL);
}

void System::deleteMainPCB()
{
	delete running;
}

void System::createIdlePCB()
{
	idle = new PCB(32, 1, NULL);
	idle->initIdlePCB();
}

void System::deleteIdlePCB()
{
	delete idle;
}

void System::createKernelPCB()
{
	kernel = new PCB(2048, 1, NULL);
	kernel->initKernelPCB();
}

void System::deleteKernelPCB()
{
	delete kernel;
}

void System::semTick()
{
	for(ID i = 0; i<vectorSem->getLength(); i++)
	{
		SystemObject *sysObj = vectorSem->get(i);
		if(sysObj != NULL)
		{
			((KernelSem*)sysObj)->notifyTimerTick();
		}
	}
}

void interrupt timer(...)
{
	tick();
	System::semTick();

	if((System::systemMode)   ||   (System::running->timeSlice == 0))
	{
		return;
	}


	AssertWithMsg(System::running != NULL, "System::running can NOT be NULL!\n");

	System::running->remaining--;
	if(System::running->remaining <= 0   &&   ! System::lockFlag)
	{
			// save registers
#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tbp, bp
				mov tsp, sp
				mov tss, ss
			}
#endif
			System::running->bp = tbp;
			System::running->sp = tsp;
			System::running->ss = tss;

			if(System::running != System::idle  &&  System::running != NULL)
			{
				Scheduler::put((PCB*)System::running);
			}
			System::running = Scheduler::get();
			if(System::running == NULL)
			{
				System::running = System::idle;
			}

			// restore registers
			tbp = System::running->bp;
			tsp = System::running->sp;
			tss = System::running->ss;
#ifndef BCC_BLOCK_IGNORE
			asm{
					mov bp, tbp
					mov sp, tsp
					mov ss, tss
			}
#endif

			((PCB*)System::running)->resetTimeSlice();
	}

	System::oldTimerInterrupt();
}

void System::dispatch()
{
	SystemCallInfo sysCallInfo = SystemCallInfo();
	(&sysCallInfo)->callType = DISPATCH;
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

void System::dispatchSystem()
{
	if(running != idle)
	{
		AssertWithMsg(running != NULL, "Running je NULL");
		Scheduler::put((PCB*)running);
	}

	running = Scheduler::get();
	if(running == NULL)
	{
		running = idle;
	}

	((PCB*)System::running)->resetTimeSlice();
}


void System::dispatchBlocked()
{
	running = Scheduler::get();
	if(running == NULL)
	{
		running = idle;
	}

	((PCB*)System::running)->resetTimeSlice();
}

void interrupt systemCall(...)
{
	System::systemMode = TRUE;

#ifndef BCC_BLOCK_IGNORE
	asm{
		// cuva sp
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
		mov tseg, cx
		mov toff, dx
	}
#endif

	System::running->sp = tsp;
	System::running->ss = tss;
	System::running->bp = tbp;

	tsp = System::kernel->sp;
	tss = System::kernel->ss;
	tbp = System::kernel->bp;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov sp, tsp // restore kernel sp
		mov ss, tss
		mov bp, tbp
	}
#endif

	sysCallInfo = (SystemCallInfo*)MK_FP(tseg, toff);
	System::executeSystemCall();

	tsp = System::running->sp;
	tss = System::running->ss;
	tbp = System::running->bp;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov sp, tsp
		mov ss, tss
		mov bp, tbp
	}
#endif

	System::systemMode = FALSE;
}

void System::executeSystemCall()
{
	switch(sysCallInfo->callType)
	{
	case THREAD_CREATE:

		PCB* pcbTC = new PCB(sysCallInfo->stackSize, sysCallInfo->timeSlice, (Thread*)sysCallInfo->caller);
		sysCallInfo->id = System::vectorPCB->put(pcbTC);

		break;

	case THREAD_START:

		((PCB*)System::vectorPCB->get(sysCallInfo->id))->start();

		break;

	case THREAD_DESTRUCT:

		PCB* pcbTD = (PCB*)System::vectorPCB->remove(sysCallInfo->id);
		delete pcbTD;

		break;

	case THREAD_WAITTOCOMPLETE:

		((PCB*)(System::vectorPCB->get(sysCallInfo->id)))->waitToComplete();

		break;

	case THREAD_GETRUNNINGID:

		sysCallInfo->runningID = vectorPCB->getID((PCB*)System::running);

		break;

	case THREAD_GETTHREADBYID:

		Thread* threadTGT = ((PCB*)vectorPCB->get(sysCallInfo->id))->owner;
		sysCallInfo->threadByID = (void*)threadTGT;

		break;

	case THREAD_FINISH:

		running->sem->signalAll();
		delete[] running->stack;
		running->stack = 0;
		running->state = FINISHED;
		running->completed = TRUE;
		dispatchBlocked();

		break;

	case DISPATCH:

		System::dispatchSystem();

		break;

	case DISPATCH_BLOCKED:

		System::dispatchBlocked();

		break;

	case SEMAPHORE_CREATE:

		KernelSem* semSC = new KernelSem(sysCallInfo->semInitValue);
		sysCallInfo->id = System::vectorSem->put(semSC);

		break;

	case SEMAPHORE_WAIT:

		KernelSem* semSW = (KernelSem*)(vectorSem->get(sysCallInfo->id));
		semSW->wait(sysCallInfo->semWaitTime);

		break;

	case SEMAPHORE_SIGNAL:

		KernelSem* semSG = (KernelSem*)(vectorSem->get(sysCallInfo->id));
		semSG->signal(sysCallInfo->semSignalValue);

		break;

	case SEMAPHORE_VALUE:

		KernelSem* semSV = (KernelSem*)(vectorSem->get(sysCallInfo->id));
		sysCallInfo->semValue = semSV->val();

		break;

	case SEMAPHORE_DESTRUCT:

		delete vectorSem->remove(sysCallInfo->id);

		break;

	case EVENT_CREATE:

		sysCallInfo->id = vectorKEvent->put(new KernelEv(sysCallInfo->ivtNo));

		break;

	case EVENT_WAIT:

		((KernelEv*)vectorKEvent->get(sysCallInfo->id))->wait();

		break;

	case EVENT_SIGNAL:

		((KernelEv*)vectorKEvent->get(sysCallInfo->id))->signal();

		break;

	case EVENT_DESTRUCT:

		delete vectorKEvent->remove(sysCallInfo->id);

		break;

	default:

		break;
	}
}
