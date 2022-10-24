#ifndef H_QUEUE_H_
#define H_QUEUE_H_

#include "basetype.h"

class SystemObject;

struct Elem
{
	SystemObject *val;
	Elem *next;
	Elem *prev;
};

class Queue
{
public:

	Elem *first, *end;
	int len;

	Queue();
	~Queue();

	void insert(SystemObject *sysObj);
	SystemObject* remove();
	BOOLEAN removeSysObj(SystemObject *sysObj);

	int size() const;
};



#endif /* H_QUEUE_H_ */
