#ifndef H_BASETYPE_H_
#define H_BASETYPE_H_

#include <assert.h>


///////////////////////////////////////////////
/*				TYPES                        */

/******** BASE *********/
typedef	unsigned int 	BOOLEAN;
typedef signed char		intS1;
typedef signed int		intS2;
typedef signed long		intS4;

typedef unsigned char	intU1;
typedef unsigned int	intU2;
typedef unsigned long	intU4;

typedef float			float4;
typedef double			float8;

#define TRUE 1;
#define FALSE 0;

#ifndef NULL
#  if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#    define NULL    0
#  else
#    define NULL    0L
#  endif
#endif
/***********************/


/********* PCB *********/
typedef intU2* 			Stack;
typedef intU4 			StackSize;
typedef void (*Runnable)();

enum STATE {START = 0, READY, RUNNING, BLOCKED, FINISHED, KERNEL, IDLE};

/***********************/


/********* IDs *********/
typedef intU4 			ID;
/***********************/


/******* SYSTEM ********/
typedef intU2 			REG;
typedef intU2			Flags;
typedef intS4			Time; 	// time, x 55ms
/***********************/


/****** INTERRUPT ******/
typedef intU1 			IVTNo;
typedef void interrupt (*IntRoutine) (...);
#define IVT_SIZE 256
/***********************/


/******* VECTOR ********/
typedef unsigned VECTOR_SIZE;
typedef float OCCUPIED_RATIO;
/***********************/


/*********************************************/
///////////////////////////////////////////////


#define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
#define FP_OFF( fp )( (unsigned )( fp ))

#define MK_FP( seg,ofs )( (void _seg * )( seg ) +( void near * )( ofs ))

#define INTE; { asm sti; }
#define INTD; { asm cli; }

#define PUSHF; { asm pushf; }
#define POPF;  { asm popf;  }

#define lock System::lockFlag = TRUE;
#define unlock System::lockFlag = FALSE;

#define SYSCALLINT; { asm int 60h; }
enum CALLTYPE { THREAD_CREATE = 0, THREAD_START, THREAD_DESTRUCT, THREAD_WAITTOCOMPLETE, THREAD_GETRUNNINGID, THREAD_GETTHREADBYID, THREAD_FINISH, DISPATCH, DISPATCH_BLOCKED, SEMAPHORE_CREATE, SEMAPHORE_WAIT, SEMAPHORE_SIGNAL, SEMAPHORE_VALUE, SEMAPHORE_DESTRUCT, EVENT_CREATE, EVENT_WAIT, EVENT_SIGNAL, EVENT_DESTRUCT };
struct SystemCallInfo
{
	SystemCallInfo()
	{
		caller = 0;
		threadByID = 0;
		stackSize = 0;
		timeSlice = 0;
		id = 0;
		runningID = 0;
		semInitValue = 0;
		semValue = 0;
		semWaitTime = 0;
		semSignalValue = 0;
		ivtNo = 0;
	}

	ID id;
	ID runningID;
	CALLTYPE callType;
	void *caller;
	void *threadByID;
	StackSize stackSize;
	Time timeSlice;
	intU2 semInitValue;
	intU2 semValue;
	Time semWaitTime;
	int semSignalValue;
	IVTNo ivtNo;
};

///////////////////////////////////////////////
/*			  ASSERTS                        */
#define AssertWithMsg(p, msg) \
( \
	(p) ? (void) 0 \
		: (void) __assertfail("Assert failed: " msg "\nCondition: %s,\nFile %s, Line %d" _ENDL, #p, __FILE__, __LINE__ ) \
)

/*********************************************/
///////////////////////////////////////////////


#endif /* H_BASETYPE_H_ */
