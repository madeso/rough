#include "rough/double.h"

#include <cmath>

constexpr double epsilon = 1e-6;


bool isZero(double d)
{
    return std::abs(d) < epsilon;
}

bool isSame(double lhs, double rhs)
{
    return std::abs(lhs - rhs) < epsilon;
}
