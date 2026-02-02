#include "core/formatting.hpp"

#include <sstream>

namespace core
{
    namespace detail
    {
        std::string FormatLogLine(const std::string& timestamp,
            const std::string& appName,
            const std::string& level,
            const std::string& message)
        {
            std::ostringstream oss;
            oss << "[" << timestamp << "] "
                << "[" << appName << "] "
                << "[" << level << "] "
                << message;
            return oss.str();
        }
    }
}
