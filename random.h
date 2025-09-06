#pragma once

#include <cmath>

inline float randomFloat(float min, float max)
{
    return (((float)rand() / (float)RAND_MAX) * (max - min)) + min;
}

