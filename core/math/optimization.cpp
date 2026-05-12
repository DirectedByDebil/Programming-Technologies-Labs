#include <iostream>
#include <string>
//#include <cmath>
//#include <random>
//#include <functional>

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

/*

std::pair<double, double> boltzmann(double T_start, double T_end, 
                                        double alpha, int iterations_per_temp) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    double x = randomPoint(borders.x_min, borders.x_max, gen);
    double y = randomPoint(borders.y_min, borders.y_max, gen);
    double E = func(x, y);
    
    double T = T_start;
    
    while (T > T_end) {
        for (int i = 0; i < iterations_per_temp; ++i) {
            // Генерация нового состояния (соседняя точка)
            double x_new = x + randomNormalDistribution(gen);
            double y_new = y + randomNormalDistribution(gen);
            
            // Ограничения
            x_new = std::clamp(x_new, borders.x_min, borders.x_max);
            y_new = std::clamp(y_new, borders.y_min, borders.y_max);
            
            double E_new = func(x_new, y_new);
            double deltaE = E_new - E;
            
            // Вероятность принятия плохого решения (Больцмана)
            double p = std::exp(-deltaE / T);
            
            if (deltaE < 0 || dis(gen) < p) {
                x = x_new;
                y = y_new;
                E = E_new;
            }
        }
        T *= alpha;  // Охлаждение
    }
    
    return {x, y};
}

*/

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
    
    while (T > simulation.T_end) {
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
        iter++;
    }
    
    return {x, y};
}

/*

std::pair<double, double> cauchy(double T_start, double T_end, 
                                    int iterations_per_temp) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::cauchy_distribution<> cauchy(0.0, 1.0);
    
    double x = randomPoint(borders.x_min, borders.x_max, gen);
    double y = randomPoint(borders.y_min, borders.y_max, gen);
    double E = func(x, y);
    
    double T = T_start;
    int iter = 0;
    
    while (T > T_end) {
        for (int i = 0; i < iterations_per_temp; ++i) {
            // Генерация нового состояния (распределение Коши)
            double x_new = x + cauchy(gen) * T;
            double y_new = y + cauchy(gen) * T;
            
            x_new = std::clamp(x_new, borders.x_min, borders.x_max);
            y_new = std::clamp(y_new, borders.y_min, borders.y_max);
            
            double E_new = func(x_new, y_new);
            double deltaE = E_new - E;
            
            // Распределение Коши даёт больше шума на ранних этапах
            double p = 1.0 / (1.0 + std::exp(deltaE / T));
            
            if (deltaE < 0 || dis(gen) < p) {
                x = x_new;
                y = y_new;
                E = E_new;
            }
        }
        T = T_start / (1 + iter);  // Охлаждение по Коши (медленнее)
        iter++;
    }
    
    return {x, y};
}

*/