#include "ksemaphr.h"

#include <iostream.h>
#include "system.h"
#include "vector.h"
#include "SCHEDULE.H"
#include "queue.h"
#include "pcb.h"
#include "thread.h"

intU2 KernelSem::NumOfSems = 0;

KernelSem::KernelSem(intS2 init)
{
	value = init;
	valueBlockedQueue = new Queue();
	timeBlockedQueue = new Queue();
}

KernelSem::~KernelSem()
{
	delete valueBlockedQueue;
	delete timeBlockedQueue;
}


intS2 KernelSem::wait(Time maxTimeToWait /* = 0 */)
{
	if(maxTimeToWait < 0)
	{
		return maxTimeToWait;
	}

	if(--value<0)
	{
		if(maxTimeToWait > 0)
		{
			((PCB*)System::running)->timeToWaitOnSem = maxTimeToWait;
			timeBlockedQueue->insert((PCB*)System::running);
		}

		System::running->state = BLOCKED;
		valueBlockedQueue->insert((PCB*)System::running);
		System::dispatchBlocked();

		return ((PCB*)System::running)->unblockedBeforeTimeElapsed;
	}

	return 1;  // 1 jer se 0 vraca samo ako je deblokirana zbog isteka vremena
			   // ovaj return je u slucaju da se nit ne zablokira na semaforu
}

intS2 KernelSem::signal(int n /* = 0 */)
{
	if(n < 0)
	{
		return n;
	}

	int unblockedThreads = 0;
	while(value++ < 0)
	{
		PCB *pcb = (PCB*)valueBlockedQueue->remove();
		if(pcb)
		{
			pcb->unblockedBeforeTimeElapsed = timeBlockedQueue->removeSysObj(pcb);

			pcb->state = READY;
			Scheduler::put(pcb);

			if(++unblockedThreads == n)
			{
				break;
			}
		}
	}

	return unblockedThreads;
}

void KernelSem::signalAll()
{
	while(value < 0)
	{
		signal();
	}
}

int KernelSem::val() const
{
	return value;
}

void KernelSem::notifyTimerTick()
{
	Elem* curr = timeBlockedQueue->first;
	Elem* next = NULL;
	while(curr != NULL)
	{
		next = curr->next;
		PCB* currPCB = (PCB*)curr->val;
		if( --currPCB->timeToWaitOnSem == 0)
		{
			timeBlockedQueue->removeSysObj(currPCB);
			valueBlockedQueue->removeSysObj(currPCB);
			currPCB->unblockedBeforeTimeElapsed = FALSE;

			currPCB->state = READY;
			Scheduler::put(currPCB);
		}

		curr = next;
	}
}
