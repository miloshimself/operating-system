#include "vector.h"

#include "sysobj.h"

VectorSysObj::VectorSysObj(VECTOR_SIZE size, OCCUPIED_RATIO ratio)
{
	vector = new SystemObject*[size];
	length = size;
	occupied = 0;
	occupiedRatio = ratio;
	for(int i = 0; i<length; i++)
	{
		vector[i] = NULL;
	}
	iterID = 0;
	lastOccupiedID = 0;
}

VectorSysObj::~VectorSysObj()
{
	delete []vector;
	length = occupied = iterID = 0;
	lastOccupiedID = 0;
}

ID VectorSysObj::put(SystemObject *systemObject)
{
	if(shouldExpand())
	{
		expand();
	}

	ID id = getNextID();
	occupied++;
	vector[id] = systemObject;
	if(id>lastOccupiedID)
	{
		lastOccupiedID = id;
	}

	return id;
}

SystemObject* VectorSysObj::get(ID id)
{
	return vector[id];
}

ID VectorSysObj::getID(SystemObject* sysObj)
{
	for(int i=0; i<=lastOccupiedID; i++)
	{
		if(vector[i] == sysObj)
		{
			return i;
		}
	}

	return 9999;
}

SystemObject* VectorSysObj::remove(ID id)
{
	if(vector[id] == NULL)
	{
		return NULL;
	}

	SystemObject *temp = vector[id];
	occupied--;
	vector[id] = NULL;
	lastOccupiedID = getLastOccupiedID();

	if(shouldContract())
	{
		contract();
	}

	return temp;
}

VECTOR_SIZE VectorSysObj::getLength()
{
	return length;
}

BOOLEAN VectorSysObj::shouldExpand()
{
	return (float)occupied/length > occupiedRatio;
}

BOOLEAN VectorSysObj::shouldContract()
{
	return (float)lastOccupiedID/length < occupiedRatio;
}

void VectorSysObj::expand()
{
	VECTOR_SIZE newLength = 2*length;
	SystemObject **temp = new SystemObject*[newLength];

	ID i = 0;
	for(i = 0; i<length; i++)
	{
		temp[i] = vector[i];
	}
	for(i = length; i<newLength; i++)
	{
		temp[i] = NULL;
	}

	delete []vector;
	length = newLength;
	vector = temp;
}

void VectorSysObj::contract()
{
	VECTOR_SIZE newLength = lastOccupiedID+10;
	SystemObject **temp = new SystemObject*[newLength];

	for(ID i = 0; i<newLength; i++)
	{
		temp[i] = vector[i];
	}

	delete []vector;
	length = newLength;
	vector = temp;
}

ID VectorSysObj::getNextID()
{
	while(vector[iterID] != NULL)
	{
		iterID = (iterID+1) % length;
	}

	return iterID;
}

ID VectorSysObj::getLastOccupiedID()
{
	if(vector[lastOccupiedID] != NULL)
	{
		return lastOccupiedID;
	}

	while(vector[lastOccupiedID] == NULL && lastOccupiedID>=0)
	{
		lastOccupiedID--;
	}

	return lastOccupiedID;
}

