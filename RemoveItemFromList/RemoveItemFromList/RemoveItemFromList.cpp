// RemoveItemFromList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListDefinition.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

PNODE RemoveValueFromList(PNODE head, int value)
{
	NODE tempHead;
	tempHead.pNext = head;

	PNODE pCurrent = head;
	PNODE pPrevious = &tempHead;

	while (NULL != pCurrent)
	{
		if (pCurrent->iNumber == value)
		{
			PNODE ptemp = pCurrent;
			pPrevious->pNext = pCurrent->pNext;
			pCurrent = pCurrent->pNext;
			delete ptemp;
		}
		else
		{
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}

	return tempHead.pNext;
}
