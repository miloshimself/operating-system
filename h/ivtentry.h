#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "basetype.h"

class KernelEv;

class IVTEntry
{
public:
	IVTEntry(IVTNo _ivtNo, IntRoutine newInterrupt);
	~IVTEntry();


	void signal();
	void callOld();

	static void restoreInterrupts();
	static void createIVTNo(IVTNo ivtNo, IntRoutine newInterrupt);

private:
	IVTNo ivtNo;
	IntRoutine oldInterrupt;

	static IVTEntry* ivtEntryList[256];
};





#endif /* H_IVTENTRY_H_ */
