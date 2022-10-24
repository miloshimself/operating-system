#ifndef H_SYSTEM_H_
#define H_SYSTEM_H_

#include "basetype.h"

class PCB;
class KernelSem;
class VectorSysObj;
class KernelEv;

class System
{
public:

	static void initSystem();
	static void restoreSystem();

	static void initSystemFiles();
	static void deleteSystemFiles();

	static void initTimer();
	static void restoreTimer();

	static void initSystemCall();
	static void restoreSystemCall();

	static void createMainPCB();
	static void deleteMainPCB();

	static void createIdlePCB();
	static void deleteIdlePCB();

	static void createKernelPCB();
	static void deleteKernelPCB();

	static void dispatch();
	static void dispatchSystem();

	static void dispatchBlocked();

	static void executeSystemCall();

	static void semTick();

	static const IVTNo IVTNo_Timer;
	static const IVTNo IVTNo_SystemCall;

	static IntRoutine oldTimerInterrupt;
	static IntRoutine oldSystemCallInterrupt;

	static BOOLEAN lockFlag;
	static BOOLEAN systemMode;

	static PCB* volatile running;
	static PCB* kernel;
	static PCB* idle;

	static VectorSysObj* vectorPCB;
	static VectorSysObj* vectorSem;
	static VectorSysObj* vectorKEvent;
	static KernelEv* KernelEvList[256];
};

#endif /* H_SYSTEM_H_ */
