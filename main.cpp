#include "core/logger.h"
#include "math/aggregates.h"
#include "text/format.h"
#include "time/clock.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    core::Logger logger("SimpleCPPApp");

    logger.Info("App starting at " + timeutil::NowUtcIso8601());

    std::string name = "Docker World";
    if (argc >= 2)
    {
        name = argv[1];
    }

    std::vector<double> samples{ 3.1, 1.5, 4.2, 1.0, 5.9, 2.6, 5.3 };
    const auto stats = mathutil::ComputeStats(samples);

    logger.Info("Hello, " + name + "!");
    logger.Info("Values: " + textutil::Join(samples, ", "));
    logger.Info("Min: " + textutil::ToString(stats.min)
        + " | Max: " + textutil::ToString(stats.max)
        + " | Avg: " + textutil::ToString(stats.average));

    const auto moving = mathutil::MovingAverage(samples, 3);
    logger.Info("Moving avg(3): " + textutil::Join(moving, ", "));

    logger.Info("Sleeping 150 ms before exit");
    timeutil::SleepMs(150);

    logger.Info("App finished at " + timeutil::NowUtcIso8601());
    return 0;
}

