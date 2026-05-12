#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <random>

namespace math_utils 
{
    template <typename T>
    T randomNumber(T min = 0, T max = 100);

    double randomPoint(const double min, const double max, std::mt19937* gen = nullptr);

    double randomNormalDistribution(const double min = 0.0, const double max = 1.0, std::mt19937* gen = nullptr);
}
#endif // !MATH_UTILS_H


