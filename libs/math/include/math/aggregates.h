#pragma once

#include <vector>

namespace mathutil
{
    struct Stats
    {
        double min{ 0.0 };
        double max{ 0.0 };
        double average{ 0.0 };
    };

    Stats ComputeStats(const std::vector<double>& values);
    std::vector<double> MovingAverage(const std::vector<double>& values, std::size_t windowSize);
}
