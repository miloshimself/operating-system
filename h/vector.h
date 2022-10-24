#ifndef H_VECTOR_H_
#define H_VECTOR_H_

#include "basetype.h"

class SystemObject;

#define DEFAULT_VECTOR_SIZE 30
#define DEFAULT_MAX_OCCUPIED_RATIO 0.75f


class VectorSysObj
{
public:
	VectorSysObj(VECTOR_SIZE size = DEFAULT_VECTOR_SIZE, OCCUPIED_RATIO ratio = DEFAULT_MAX_OCCUPIED_RATIO);
	~VectorSysObj();

	ID put(SystemObject* systemObject);

	SystemObject* get(ID id);
	ID getID(SystemObject* sysObj);
	SystemObject* remove(ID id);

	VECTOR_SIZE getLength();

private:

	BOOLEAN shouldExpand();
	BOOLEAN shouldContract();

	void expand();
	void contract();

	ID getNextID();
	ID getLastOccupiedID();


	SystemObject **vector;
	VECTOR_SIZE length;
	intU2 occupied;
	ID iterID;
	ID lastOccupiedID;
	OCCUPIED_RATIO occupiedRatio;
};



#endif /* H_VECTOR_H_ */
