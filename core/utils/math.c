#include <time.h>
#include <stdlib.h>

#include "math.h"


static int is_seeded = 0;

static void InitRandom(void)
{
    if (!is_seeded)
    {
        srand(time(NULL));
        is_seeded = 1;
    }
}

double RandomDouble(double min, double max)
{
    InitRandom();
    double range = max - min;
    return min + (double)rand() / RAND_MAX * range;
}
