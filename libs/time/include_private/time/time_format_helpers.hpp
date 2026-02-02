#pragma once

#include <ctime>
#include <string>

namespace timeutil
{
    namespace detail
    {
        std::string FormatUtcIso8601(const std::tm& utcTime);
    }
}
