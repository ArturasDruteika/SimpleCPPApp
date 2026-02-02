#pragma once

#include <string>

namespace timeutil
{
    std::string NowUtcIso8601();
    void SleepMs(int milliseconds);
}
