#include "text/delimiter_helpers.hpp"

namespace textutil
{
    namespace detail
    {
        std::string NormalizeDelimiter(const std::string& delimiter)
        {
            if (delimiter.empty())
            {
                return ", ";
            }
            return delimiter;
        }
    }
}
