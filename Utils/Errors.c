#include <stdlib.h>

#include "Errors.h"

void MallocErrorTest(unsigned int p)
{
    if (!p)
        exit(137);
}
