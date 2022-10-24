#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_

#include "basetype.h"
#include "sysobj.h"
#include "ksemaphr.h"
#include "pcb.h"

class KernelSem;

class KernelEv: public SystemObject
{
public:
	KernelEv(IVTNo _ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	PCB* const creator;
	IVTNo ivtNo;

	friend class IVTEntry;
	KernelSem *sem;
};



#endif /* H_KERNELEV_H_ */
