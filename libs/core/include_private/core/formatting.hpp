#pragma once

#include <string>

namespace core
{
    namespace detail
    {
        std::string FormatLogLine(const std::string& timestamp,
            const std::string& appName,
            const std::string& level,
            const std::string& message);
    }
}
