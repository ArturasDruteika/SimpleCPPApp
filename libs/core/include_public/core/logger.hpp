#pragma once

#include <memory>
#include <string>

namespace core
{
    class Logger
    {
    public:
        enum class Level
        {
            Info,
            Warning,
            Error
        };

        explicit Logger(std::string appName);
        ~Logger();

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&&) noexcept;
        Logger& operator=(Logger&&) noexcept;

        void Info(const std::string& message) const;
        void Warn(const std::string& message) const;
        void Error(const std::string& message) const;

    private:
        void Log(Level level, const std::string& message) const;
        static std::string LevelToString(Level level);
        static std::string NowUtcIso8601();

        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
