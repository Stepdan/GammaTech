#pragma once

#include "log_settings.hpp"

#include <thread>
#include <string>
#include <string_view>

#include <fmt/format.h>

namespace gamma::log {

class Logger
{
public:
    static Logger& instance();

    void set_settings(LoggingSettings&&);
    void reset();
    void set_log_level(LOG_LEVEL);
    LOG_LEVEL get_log_level() const;
    void dump_backtrace();

    template <typename... Args>
    void log(LOG_LEVEL level, const fmt::format_string<Args...>& str, Args&&... args)
    {
        if (m_logger)
            m_logger->log(level, str, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void log_to_default(LOG_LEVEL level, const fmt::format_string<Args...>& str, Args&&... args)
    {
        spdlog::log(level, str, std::forward<Args>(args)...);
    }

private:
    Logger();
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    std::shared_ptr<spdlog::logger> m_logger;
};

}  // namespace gamma::log

// {:>20.20s}; - STEPKIT_MODULE_NAME
#define GAMMA_LOG(level, format, ...)                                                                                   \
    gamma::log::Logger::instance().log(level, "{}; msg=" format " ({}:{}, {})", GAMMATECH_MODULE_NAME, ##__VA_ARGS__,     \
                                      __FILE__, __LINE__, __FUNCTION__)

#define GAMMA_LOG_TO_DEFAULT_NO_SOURCE(level, format, ...)                                                              \
    gamma::log::Logger::instance().log_to_default(level, "{}; msg=" format, GAMMATECH_MODULE_NAME, ##__VA_ARGS__)

#define GAMMA_LOG_TO_DEFAULT(level, format, ...)                                                                        \
    gamma::log::Logger::instance().log_to_default(level, "{}; msg=" format " ({}:{}, {})", GAMMATECH_MODULE_NAME,         \
                                                 ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
