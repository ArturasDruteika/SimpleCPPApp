#pragma once

#include <string>
#include <vector>

namespace textutil
{
    std::string ToString(double value, int precision = 2);
    std::string ToUpper(std::string value);
    std::string Join(const std::vector<double>& values, const std::string& delimiter);
}
