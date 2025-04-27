/*
MIT License

Copyright (c) 2024 TaciturnJian@github.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef INCLUDE_CANGO_LOGGING
#define INCLUDE_CANGO_LOGGING

#include <chrono>
#include <concepts>
#include <exception>
#include <format>
#include <memory>
#include <ostream>
#include <source_location>
#include <stacktrace>
#include <string_view>

namespace cango::logging::details::code_quality_guarantee::cpp_standard {
// TODO: check cpp version and cpp standard, make sure all the feature in this library is supported
}

namespace cango::logging {
template<typename TFunc>
concept OutputFunc = std::is_nothrow_invocable_v<TFunc, std::string_view>;

enum class log_level {
    trace = -2,
    debug = -1,
    info  = 0,
    warn  = 1,
    error = 2,
    fatal = 3
};

constexpr const char *get_str(const log_level level) noexcept {
    switch (level) {
    case log_level::trace:
        return "trace";
    case log_level::debug:
        return "debug";
    case log_level::info:
        return "info";
    case log_level::warn:
        return "warn";
    case log_level::error:
        return "error";
    case log_level::fatal:
        return "fatal";
    default:
        return "UNKNOWN";
    }
}

/// @brief 利用提供的函数输出日志消息，包含时间戳、日志等级、日志器名称，也可以提供额外的异常与堆栈、源码信息
/// @details 输出的日志格式为：
///     [UTC时间][日志等级](日志器名称)> 日志消息
///     可选的源文件信息
///     可选的异常信息
///     当异常信息存在时，堆栈信息会被附加到日志消息的末尾
/// @tparam TOutputFunc 输出函数类型，要求是一个可调用无异常对象，接受一个 @c std::string_view 参数
template<OutputFunc TOutputFunc>
class CangoLogger {
    TOutputFunc Output;
    std::string Name;

public:
    CangoLogger(TOutputFunc output, const std::string_view name) noexcept :
        Output(output), Name(name) {}

    template<typename... Args>
    void log(const log_level level, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        auto time_str = std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::utc_clock::now());
        auto message = std::format<Args...>(fmt, std::forward<Args>(args)...);
        auto endl = message.ends_with('\n') ? "" : "\n";
        Output(
            std::format(
                "[{}][{}]({})> {}{}",
                std::move(time_str),
                get_str(level),
                Name,
                std::move(message),
                std::move(endl)
            )
        );
    }

    template<typename... Args>
    void log(
        const log_level level,
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept {
        const auto trace = std::stacktrace::current();
        auto message = std::format(fmt, std::forward<Args>(args)...);
        log(level, "{}\nException:{}\nStacktrace:\n{}\n\n", std::move(message), ex.what(), std::to_string(trace));
    }

    template<typename... Args>
    void log(
        const log_level level,
        const std::source_location &location,
        const std::format_string<Args...> fmt,
        Args &&... args
    ) const noexcept {
        auto message = std::format(fmt, std::forward<Args>(args)...);
        log(level, "{}\nSource: {}:{}", std::move(message), location.file_name(), location.line());
    }

    template<typename... Args>
    void log(
        const log_level level,
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args
    ) const noexcept {
        const auto trace = std::stacktrace::current();
        auto message = std::format(fmt, std::forward<Args>(args)...);
        log(level, "{}\nSource: {}:{}\nException: {}\nStacktrace:\n{}\n\n",
            std::move(message),
            location.file_name(),
            location.line(),
            ex.what(),
            std::to_string(trace)
        );
    }

    template<bool condition, typename... Args>
    void log_if(const log_level level, const std::format_string<Args...> format, Args &&... args) const noexcept {
        if constexpr (condition) { log(level, format, std::forward<Args>(args)...); }
    }

    template<bool condition, typename... Args>
    void log_if(const log_level level, const std::source_location &location, Args &&... args) const noexcept {
        if constexpr (condition) { log(level, location, std::forward<Args>(args)...); }
    }

    template<bool condition, typename... Args>
    void log_if(
        const log_level level,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept {
        if constexpr (condition) { log(level, ex, format, std::forward<Args>(args)...); }
    }

    template<bool condition, typename... Args>
    void log_if(
        const log_level level,
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept {
        if constexpr (condition) { log(level, location, ex, format, std::forward<Args>(args)...); }
    }

    template<typename... Args>
    void log_if(
        const bool condition,
        const log_level level,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept {
        if (!condition) return;
        log(level, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log_if(
        const bool condition,
        const log_level level,
        const std::source_location &location,
        Args &&... args) const noexcept {
        if (!condition) return;
        log(level, location, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log_if(
        const bool condition,
        const log_level level,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept {
        if (!condition) return;
        log(level, ex, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log_if(
        const bool condition,
        const log_level level,
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept {
        if (!condition) return;
        log(level, location, ex, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void trace(const std::format_string<Args...> format, Args &&... args) const noexcept {
        log(log_level::trace, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(const std::format_string<Args...> format, Args &&... args) const noexcept {
        log(log_level::debug, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log(log_level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log(log_level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log(log_level::error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void fatal(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log(log_level::fatal, fmt, std::forward<Args>(args)...);
    }

    template<bool condition, typename... Args>
    void trace_if(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if<condition>(log_level::trace, fmt, std::forward<Args>(args)...);
    }

    template<bool condition, typename... Args>
    void debug_if(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if<condition>(log_level::debug, fmt, std::forward<Args>(args)...);
    }

    template<bool condition, typename... Args>
    void info_if(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if<condition>(log_level::info, fmt, std::forward<Args>(args)...);
    }

    template<bool condition, typename... Args>
    void warn_if(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if<condition>(log_level::warn, fmt, std::forward<Args>(args)...);
    }

    template<bool condition, typename... Args>
    void error_if(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if<condition>(log_level::error, fmt, std::forward<Args>(args)...);
    }

    template<bool condition, typename... Args>
    void fatal_if(const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if<condition>(log_level::fatal, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void trace_if(const bool condition, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if(condition, log_level::trace, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug_if(const bool condition, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if(condition, log_level::debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info_if(const bool condition, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if(condition, log_level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn_if(const bool condition, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if(condition, log_level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error_if(const bool condition, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if(condition, log_level::error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void fatal_if(const bool condition, const std::format_string<Args...> fmt, Args &&... args) const noexcept {
        log_if(condition, log_level::fatal, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void trace(
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::trace, ex, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void trace(
        const std::source_location &location,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::trace, location, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void trace(
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::trace, location, ex, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void debug(
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::debug, ex, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void debug(
        const std::source_location &location,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::debug, location, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void debug(
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::debug, location, ex, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void info(
        const std::exception &ex,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::info, ex, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void info(
        const std::source_location &location,
        const std::format_string<Args...> fmt,
        Args &&... args) const noexcept { log(log_level::info, location, fmt, std::forward<Args>(args)...); }

    template<typename... Args>
    void info(
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::info, location, ex, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void warn(
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::warn, ex, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void warn(
        const std::source_location &location,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::warn, location, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void warn(
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::warn, location, ex, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void error(
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::error, ex, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void error(
        const std::source_location &location,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::error, location, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void error(
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::error, location, ex, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void fatal(
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::fatal, ex, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void fatal(
        const std::source_location &location,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::fatal, location, format, std::forward<Args>(args)...); }

    template<typename... Args>
    void fatal(
        const std::source_location &location,
        const std::exception &ex,
        const std::format_string<Args...> format,
        Args &&... args) const noexcept { log(log_level::fatal, location, ex, format, std::forward<Args>(args)...); }
};

namespace details {

/// @brief 给定输出流，返回输出函数
/// @note 如果你在日志的堆栈中看到了此函数，请找 CangoLogger 内部的 log 函数，这个函数意在缩短这个 lambda 匿名函数的名称
inline auto AtCangoLogger(std::ostream &stream) noexcept {
    return [&stream](const std::string_view message) noexcept { stream << message; };
}
}

template<OutputFunc TOutputFunc>
std::unique_ptr<CangoLogger<TOutputFunc> > make_unique_cango_logger(
    TOutputFunc output, const std::string_view name) noexcept {
    return std::make_unique<CangoLogger<TOutputFunc> >(std::move(output), name);
}

inline auto make_unique_cango_logger(std::ostream &stream, const std::string_view name) noexcept {
    return make_unique_cango_logger(details::AtCangoLogger(stream), name);
}

template<OutputFunc TOutputFunc>
std::shared_ptr<CangoLogger<TOutputFunc> > make_shared_cango_logger(
    TOutputFunc output, const std::string_view name) noexcept {
    return std::make_shared<CangoLogger<TOutputFunc> >(std::move(output), name);
}

inline auto make_shared_cango_logger(std::ostream &stream, const std::string_view name) noexcept {
    return make_shared_cango_logger(details::AtCangoLogger(stream), name);
}
}

#endif//INCLUDE_CANGO_LOGGING
