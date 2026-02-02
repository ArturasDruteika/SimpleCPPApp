#pragma once

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

        void Info(const std::string& message) const;
        void Warn(const std::string& message) const;
        void Error(const std::string& message) const;

    private:
        std::string appName_;

        void Log(Level level, const std::string& message) const;
        static std::string LevelToString(Level level);
        static std::string NowUtcIso8601();
    };
}
