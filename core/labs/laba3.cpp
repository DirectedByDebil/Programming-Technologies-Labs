#include <iostream>

#include "laba.h"
#include "../math/optimization.h"

double test (double x, double y)
{
    return x + y;
}

int cpp_labs::startLab3(int argc, char* argv [])
{
    math_labs::SimulationParams simulation;
    math_labs::BorderParams borders;

    math_labs::simulateBoltzmann(test, simulation, borders);
    math_labs::simulateCauchy(test, simulation, borders);
    return 0;
}
