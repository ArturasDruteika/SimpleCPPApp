#include "core/logger.hpp"

#include "core/formatting.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

namespace core
{
    struct Logger::Impl
    {
        explicit Impl(std::string name)
            : appName(std::move(name))
        {
        }

        std::string appName;
    };

    Logger::Logger(std::string appName)
        : impl_(std::make_unique<Impl>(std::move(appName)))
    {
    }

    Logger::~Logger() = default;

    Logger::Logger(Logger&& other) noexcept = default;

    Logger& Logger::operator=(Logger&& other) noexcept = default;

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
        const auto line = detail::FormatLogLine(
            NowUtcIso8601(),
            impl_ ? impl_->appName : "unknown",
            LevelToString(level),
            message);
        std::cout << line << "\n";
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
