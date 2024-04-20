#include <stdio.h>
#include <stdlib.h>

#include "../Utils/Headers/UtilsHeaders.h"


void MallocErrorTest(unsigned int p)
{
	if (!p)
		exit(137);
}