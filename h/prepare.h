#ifndef H_PREPARE_H_
#define H_PREPARE_H_

#include "basetype.h"
#include "ivtentry.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "system.h"

#define PREPAREENTRY(ivtNo, old)\
	void interrupt Ievent##ivtNo(...);\
	\
	static IVTEntry Ientry##ivtNo = IVTEntry(ivtNo, Ievent##ivtNo);\
	\
	void interrupt Ievent##ivtNo(...)\
	{\
		Ientry##ivtNo.signal();\
		if(old)\
			Ientry##ivtNo.callOld();\
	}




#endif /* H_PREPARE_H_ */
