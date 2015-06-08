/**
 * Log
 */

#ifndef MASSIVELY_AUTH__LOG__H_
#define MASSIVELY_AUTH__LOG__H_

#include <string>

#include <spdlog/spdlog.h>

namespace massively {
namespace auth      {

class Log {
public:
    Log(std::string const &topic)
    : m_log(spdlog::stdout_logger_mt(topic))
    { }

    ~Log() { }
    template <typename... Args>
    inline void trace(const char* fmt, const Args&... args) { m_log->trace(fmt, args...); };

    template <typename... Args>
    inline void debug(const char* fmt, const Args&... args) { m_log->debug(fmt, args...); };

    template <typename... Args>
    inline void info(const char* fmt, const Args&... args) { m_log->info(fmt, args...); };

    template <typename... Args>
    inline void warn(const char* fmt, const Args&... args) { m_log->warn(fmt, args...); };

    template <typename... Args>
    inline void error(const char* fmt, const Args&... args) { m_log->error(fmt, args...); };

private:
    std::shared_ptr<spdlog::logger> m_log;
};

} // end namespace auth
} // end namespace massively

#endif // MASSIVELY_AUTH__LOG__H_

