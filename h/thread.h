#ifndef H_THREAD_H_
#define H_THREAD_H_

#include "basetype.h"

const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2; // default = 2*55ms

class PCB; // Kernel's implementation of a user's thread

class Thread
{
public:
	friend class System;
	friend class KernelSem;
	friend class KernelEv;
	friend void interrupt systemCall(...);
	friend void interrupt timer(...);

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

protected:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run();

private:
	ID myPCB;

	static void wrapper();
};

void dispatch ();

#endif /* H_THREAD_H_ */
