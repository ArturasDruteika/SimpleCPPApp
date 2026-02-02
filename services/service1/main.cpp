#include "core/logger.h"
#include "math/aggregates.h"
#include "time/clock.h"

#include <iostream>
#include <random>
#include <vector>

int main()
{
    core::Logger logger("service1");
    logger.Info("Service1 booting at " + timeutil::NowUtcIso8601());

    std::mt19937 rng(42);
    std::normal_distribution<double> dist(10.0, 2.5);

    std::vector<double> samples;
    for (int i = 0; i < 8; ++i)
    {
        samples.push_back(dist(rng));
    }

    const auto stats = mathutil::ComputeStats(samples);

    logger.Info("Sample count: " + std::to_string(samples.size()));
    logger.Info("Min: " + std::to_string(stats.min));
    logger.Info("Max: " + std::to_string(stats.max));
    logger.Info("Avg: " + std::to_string(stats.average));

    logger.Info("Service1 done.");
    return 0;
}
