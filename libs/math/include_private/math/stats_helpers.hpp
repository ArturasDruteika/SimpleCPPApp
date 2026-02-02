#pragma once

#include <cstddef>
#include <vector>

namespace mathutil
{
    namespace detail
    {
        bool IsWindowValid(std::size_t windowSize, std::size_t valueCount);
        double SumRange(const std::vector<double>& values, std::size_t start, std::size_t endExclusive);
    }
}
