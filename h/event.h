#ifndef H_EVENT_H_
#define H_EVENT_H_

#include "basetype.h"

class KernelEv;

class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();
protected:
	friend class KernelEv;

	void signal(); // can call KernelEv
private:
	ID myImpl;
};

#endif /* H_EVENT_H_ */
