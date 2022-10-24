#include "kernelev.h"
#include "system.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "thread.h"

KernelEv::KernelEv(IVTNo _ivtNo) : creator(System::running)
{
	AssertWithMsg(creator != NULL, "KernelEv::wait() : Running PCB is NULL");
	ivtNo = _ivtNo;
	System::KernelEvList[ivtNo] = (KernelEv*) this;
	sem = new KernelSem(0);
}

KernelEv::~KernelEv() {
	if (sem->val() == -1) {
		sem->signal();
	}
	System::KernelEvList[ivtNo] = NULL;
	delete sem;
}

void KernelEv::wait()
{
	AssertWithMsg(System::running != NULL, "KernelEv::wait() : Running PCB is NULL");
	if (System::running == creator)
	{
		sem->wait();
	}
}

void KernelEv::signal() {
	if (sem->val() == -1) {
		sem->signal();
	}
}
