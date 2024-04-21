#include "Errors.h"

#include <stdlib.h>

void MallocErrorTest(unsigned int p)
{
    if (!p)
        exit(137);
}
