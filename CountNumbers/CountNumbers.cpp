// CountNumbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	for (int iCount = 0; iCount <= 100; iCount++)
	{
		if (0 == (iCount % 15))
		{
			printf("FizzBizz\n");
		}
		else if ( 0 == (iCount % 5))
		{
			printf("Fizz\n");
		}
		else if (0 == (iCount % 3))
		{
			printf("Bizz\n");
		}
		else
		{
			printf("%i\n",iCount);
		}

	}
	return 0;
}

