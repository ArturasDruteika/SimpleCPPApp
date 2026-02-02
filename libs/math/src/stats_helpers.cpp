#include "math/stats_helpers.hpp"

#include <numeric>

namespace mathutil
{
    namespace detail
    {
        bool IsWindowValid(std::size_t windowSize, std::size_t valueCount)
        {
            return windowSize > 0 && valueCount >= windowSize;
        }

        double SumRange(const std::vector<double>& values, std::size_t start, std::size_t endExclusive)
        {
            return std::accumulate(values.begin() + static_cast<long>(start),
                values.begin() + static_cast<long>(endExclusive),
                0.0);
        }
    }
}
