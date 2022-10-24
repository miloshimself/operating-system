#include "queue.h"

#include "sysobj.h"
#include <iostream.h>
#include "system.h"
#include "pcb.h"

Queue::Queue()
{
	len = 0;
	first = end = NULL;
}

Queue::~Queue()
{
	len = 0;
	Elem *t = first;

	while(t != NULL)
	{
		t = first;
		first = first->next;
		delete t;
	}

	first = end = NULL;
}

void Queue::insert(SystemObject *sysObj)
{
	if(len++ == 0)
	{
		first = end = new Elem();
		first->prev = NULL;
	}
	else
	{
		end->next = new Elem();
		end->next->prev = end;
		end = end->next;
	}

	end->val = sysObj;
	end->next = NULL;
}

SystemObject* Queue::remove()
{
	if(len == 0)
	{
		return NULL;
	}

	len--;
	SystemObject *p = first->val;
	Elem *t = first;
	first = first->next;
	first->prev = NULL;

	delete t;
	return p;
}

BOOLEAN Queue::removeSysObj(SystemObject *sysObj)
{
	if(sysObj == NULL  ||  len == 0)
	{
		return FALSE;
	}

	Elem* curr = first;
	while(curr != NULL)
	{
		if(curr->val == sysObj)
		{
			Elem* temp = curr;

			if(curr == first)
			{
				if(curr->next != NULL)
				{
					curr->next->prev = NULL;
				}
				first = curr->next;
			}
			else if(curr == end)
			{
				if(curr->prev != NULL)
				{
					curr->prev->next = NULL;
				}
				end = curr->prev;
			}
			else
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}

			delete temp;
			len--;
			return TRUE;
		}

		curr = curr->next;
	}

	return FALSE;
}

int Queue::size() const
{
	return len;
}

