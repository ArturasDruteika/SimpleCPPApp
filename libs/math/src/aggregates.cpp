#include "math/aggregates.hpp"

#include "math/stats_helpers.hpp"

#include <algorithm>
#include <numeric>

namespace mathutil
{
    Stats ComputeStats(const std::vector<double>& values)
    {
        Stats stats{};
        if (values.empty())
        {
            return stats;
        }

        const auto minmax = std::minmax_element(values.begin(), values.end());
        stats.min = *minmax.first;
        stats.max = *minmax.second;
        stats.average = std::accumulate(values.begin(), values.end(), 0.0) / values.size();

        return stats;
    }

    std::vector<double> MovingAverage(const std::vector<double>& values, std::size_t windowSize)
    {
        std::vector<double> result;
        if (!detail::IsWindowValid(windowSize, values.size()))
        {
            return result;
        }

        result.reserve(values.size() - windowSize + 1);
        double sum = detail::SumRange(values, 0, windowSize);
        result.push_back(sum / windowSize);

        for (std::size_t i = windowSize; i < values.size(); ++i)
        {
            sum += values[i] - values[i - windowSize];
            result.push_back(sum / windowSize);
        }

        return result;
    }
}
