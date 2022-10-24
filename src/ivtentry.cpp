#include "ivtentry.h"
#include "kernelev.h"
#include "system.h"
#include <dos.h>
#include <iostream.h>

KernelEv* System::KernelEvList[256];

IVTEntry::IVTEntry(IVTNo _ivtNo, IntRoutine newInterrupt)
{
	ivtNo = _ivtNo;
#ifndef BCC_BLOCK_IGNORE
	oldInterrupt = getvect(ivtNo);
	setvect(ivtNo, newInterrupt);
#endif
}

IVTEntry::~IVTEntry()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldInterrupt);
#endif
	oldInterrupt();
}

void IVTEntry::signal(){
	if(System::KernelEvList[ivtNo] != NULL)
	{
		System::KernelEvList[ivtNo]->signal();
	}
}

void IVTEntry::callOld(){
	oldInterrupt();
}

void IVTEntry::restoreInterrupts(){

}

void IVTEntry::createIVTNo(IVTNo ivtNo, IntRoutine newInterrupt){
}
