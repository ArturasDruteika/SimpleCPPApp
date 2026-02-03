#include "time/clock.hpp"

#include "time/time_format_helpers.hpp"

#include <chrono>
#include <ctime>
#include <thread>

namespace timeutil
{
    std::string NowUtcIso8601()
    {
        using namespace std::chrono;

        const auto now = system_clock::now();
        const auto t = system_clock::to_time_t(now);

        std::tm tmUtc{};
#if defined(_WIN32)
        gmtime_s(&tmUtc, &t);
#else
        gmtime_r(&t, &tmUtc);
#endif

        return detail::FormatUtcIso8601(tmUtc);
    }

    void SleepMs(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}
