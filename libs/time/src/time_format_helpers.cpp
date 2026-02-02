#include "time/time_format_helpers.hpp"

#include <iomanip>
#include <sstream>

namespace timeutil
{
    namespace detail
    {
        std::string FormatUtcIso8601(const std::tm& utcTime)
        {
            std::ostringstream oss;
            oss << std::put_time(&utcTime, "%Y-%m-%dT%H:%M:%SZ");
            return oss.str();
        }
    }
}
