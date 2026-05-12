#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <functional>

namespace math_labs {

    using TargetFunc = double (*)(double, double);

    struct BorderParams
    {
        double x_min;
        double x_max;
        double y_min;
        double y_max;
    };
    
    struct SimulationParams
    {        
        double T_start;
        double T_end;
        int iterations_per_temp;
        double alpha;
    };

    std::pair<double, double> simulateBoltzmann(const TargetFunc targetFunc, const SimulationParams simulation, const BorderParams borders);
    std::pair<double, double> simulateCauchy(const TargetFunc targetFunc, const SimulationParams simulation, const BorderParams borders);
}

#endif