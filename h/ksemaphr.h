#ifndef H_KSEMAPHR_H_
#define H_KSEMAPHR_H_

#include "basetype.h"
#include "sysobj.h"

class Queue;

class KernelSem : public SystemObject
{
public:
	KernelSem(intS2 init);
	~KernelSem();

	intS2 wait(Time maxTimeToWait = 0);
	intS2 signal(int n = 0);
	void signalAll();

	int val() const;
	void notifyTimerTick();

private:
	volatile int value;
	Queue* valueBlockedQueue;
	Queue* timeBlockedQueue;
	static intU2 NumOfSems;
};



#endif /* H_KSEMAPHR_H_ */
