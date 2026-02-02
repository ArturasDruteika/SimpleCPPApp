#include "core/logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace core
{
    Logger::Logger(std::string appName)
        : appName_(std::move(appName))
    {
    }

    void Logger::Info(const std::string& message) const
    {
        Log(Level::Info, message);
    }

    void Logger::Warn(const std::string& message) const
    {
        Log(Level::Warning, message);
    }

    void Logger::Error(const std::string& message) const
    {
        Log(Level::Error, message);
    }

    void Logger::Log(Level level, const std::string& message) const
    {
        std::cout << "[" << NowUtcIso8601() << "] "
                  << "[" << appName_ << "] "
                  << "[" << LevelToString(level) << "] "
                  << message << "\n";
    }

    std::string Logger::LevelToString(Level level)
    {
        switch (level)
        {
        case Level::Info:
            return "INFO";
        case Level::Warning:
            return "WARN";
        case Level::Error:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    std::string Logger::NowUtcIso8601()
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

        std::ostringstream oss;
        oss << std::put_time(&tmUtc, "%Y-%m-%dT%H:%M:%SZ");
        return oss.str();
    }
}
