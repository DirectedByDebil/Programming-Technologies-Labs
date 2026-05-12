#include <random>
#include <type_traits> 
#include <random>

#include "mathUtils.h"

template <typename T>
T math_utils::randomNumber(T min, T max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen);
    } else {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(gen);
    }
}


double math_utils::randomPoint(const double min, const double max, std::mt19937* gen)
{
    static std::mt19937 defaultGen(std::random_device{}());
    if (gen == nullptr)
    {
        gen = &defaultGen;
    }
    
    std::uniform_real_distribution<double> dis(min, max);
    return dis(*gen);
}


double math_utils::randomNormalDistribution(const double min, const double max, std::mt19937* gen)
{
    static std::mt19937 defaultGen(std::random_device{}());
    if (gen == nullptr)
    {
        gen = &defaultGen;
    }
    
    std::normal_distribution<double> norm(min, max);
    return norm(*gen);
}


template int math_utils::randomNumber<int>(int, int);
template double math_utils::randomNumber<double>(double, double);