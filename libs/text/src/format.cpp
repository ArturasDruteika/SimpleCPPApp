#include "text/format.hpp"

#include "text/delimiter_helpers.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace textutil
{
    std::string ToString(double value, int precision)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        return oss.str();
    }

    std::string ToUpper(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });
        return value;
    }

    std::string Join(const std::vector<double>& values, const std::string& delimiter)
    {
        std::ostringstream oss;
        const auto safeDelimiter = detail::NormalizeDelimiter(delimiter);
        for (std::size_t i = 0; i < values.size(); ++i)
        {
            oss << ToString(values[i]);
            if (i + 1 < values.size())
            {
                oss << safeDelimiter;
            }
        }
        return oss.str();
    }
}
