#include "core/logger.h"
#include "text/format.h"
#include "time/clock.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    core::Logger logger("service2");
    logger.Info("Service2 starting at " + timeutil::NowUtcIso8601());

    std::string input = "message from service2";
    if (argc >= 2)
    {
        input = argv[1];
    }

    logger.Info("Original: " + input);
    logger.Info("Upper: " + textutil::ToUpper(input));

    logger.Info("Service2 done.");
    return 0;
}
