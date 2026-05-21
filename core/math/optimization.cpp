#include <iostream>
#include <string>
#include <cmath>

#include "optimization.h"
#include "../utils/mathUtils.h"

std::pair<double, double> math_labs::simulateBoltzmann(const TargetFunc targetFunc, const SimulationParams simulation, const BorderParams borders)
{
    //std::cout << "Boltzmann: \n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    double x = math_utils::randomPoint(borders.x_min, borders.x_max, &gen);
    double y = math_utils::randomPoint(borders.y_min, borders.y_max, &gen);
    double E = targetFunc(x, y);
    
    double T = simulation.T_start;
    
    while (T > simulation.T_end) {
        for (int i = 0; i < simulation.iterations_per_temp; ++i) {
            // Генерация нового состояния (соседняя точка)
            double x_new = x + math_utils::randomNormalDistribution(0.0, 1.0);
            double y_new = y + math_utils::randomNormalDistribution(0.0, 1.0);
            
            x_new = std::clamp(x_new, borders.x_min, borders.x_max);
            y_new = std::clamp(y_new, borders.y_min, borders.y_max);
            
            double E_new = targetFunc(x_new, y_new);
            double deltaE = E_new - E;
            
            // Вероятность принятия плохого решения (Больцмана)
            double p = std::exp(-deltaE / T);
            
            if (deltaE < 0 || dis(gen) < p) {
                x = x_new;
                y = y_new;
                E = E_new;
            }
        }
        T *=  simulation.alpha;  // Охлаждение
    }
    
    return {x, y};
}

std::pair<double, double> math_labs::simulateCauchy(const TargetFunc targetFunc, const SimulationParams simulation, const BorderParams borders)
{
    //std::cout << "Cauchy: \n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::cauchy_distribution<> cauchy(0.0, 1.0);
    
    double x = math_utils::randomPoint(borders.x_min, borders.x_max, &gen);
    double y = math_utils::randomPoint(borders.y_min, borders.y_max, &gen);
    double E = targetFunc(x, y);
    
    double T = simulation.T_start;
    int iter = 0;
    
    while (T > simulation.T_end && iter < simulation.max_iterations) {
        for (int i = 0; i < simulation.iterations_per_temp; ++i) {
            // Генерация нового состояния (распределение Коши)
            double x_new = x + cauchy(gen) * T;
            double y_new = y + cauchy(gen) * T;
            
            x_new = std::clamp(x_new, borders.x_min, borders.x_max);
            y_new = std::clamp(y_new, borders.y_min, borders.y_max);
            
            double E_new = targetFunc(x_new, y_new);
            double deltaE = E_new - E;
            
            // Распределение Коши даёт больше шума на ранних этапах
            double p = 1.0 / (1.0 + std::exp(deltaE / T));
            
            if (deltaE < 0 || dis(gen) < p) {
                x = x_new;
                y = y_new;
                E = E_new;
            }
        }
        T = simulation.T_start / (1 + iter);  // Охлаждение по Коши (медленнее)
        //T *= simulation.alpha;        

        iter++;
    }
    
    return {x, y};
}


#pragma region Target functions

double math_labs::rastrigin(double x, double y) {
    return 20 + (x*x - 10*cos(2*M_PI*x)) + (y*y - 10*cos(2*M_PI*y));
}

double math_labs::rosenbrock(double x, double y) {
    return (1 - x)*(1 - x) + 100 * (y - x*x)*(y - x*x);
}

double math_labs::ackley(double x, double y) {
    double a = 20, b = 0.2, c = 2 * M_PI;
    return -a * exp(-b * sqrt(0.5*(x*x + y*y))) 
        - exp(0.5*(cos(c*x) + cos(c*y))) + a + exp(1);
}

double math_labs::beale(double x, double y) {
    return (1.5 - x + x*y)*(1.5 - x + x*y) 
        + (2.25 - x + x*y*y)*(2.25 - x + x*y*y) 
        + (2.625 - x + x*y*y*y)*(2.625 - x + x*y*y*y);
}

#pragma endregion