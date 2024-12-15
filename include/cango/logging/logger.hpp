#pragma once

#include <chrono>
#include <format>
#include <memory>
#include <optional>

#include "output_stream.hpp"

namespace cango::logging {
    /// @brief 向流中输出格式化后的日志消息
    /// @tparam Args 格式化的参数的类型，与 @c std::format 格式相同
    /// @tparam stream_type 输出流的类型
    /// @param stream 输出流，接收格式化后的消息
    /// @param format 输出格式
    /// @param args 格式参数
    /// @return 消息长度
    template<is_output_stream stream_type, typename... Args>
    std::size_t log(stream_type &stream, const std::string_view format, Args &&... args) {
        std::string message = std::vformat(format, std::make_format_args(args...));
        stream.output(message);
        return message.size();
    }

    /// @brief 在条件成立时，向流中输出格式化后的日志消息
    /// @tparam Args 格式化的参数的类型，与 @c std::format 格式相同
    /// @tparam stream_type 输出流的类型
    /// @param condition 条件，在运行时判断
    /// @param stream 输出流，接收格式化后的消息
    /// @param format 输出格式
    /// @param args 格式参数
    /// @return 消息长度，当条件不成立时返回 0
    template<is_output_stream stream_type, typename... Args>
    std::size_t log_if(const bool condition, stream_type &stream, const std::string_view format, Args &&... args) {
        if (!condition) return 0;
        return log(stream, format, std::forward<Args>(args)...);
    }

    /// @brief 在条件成立时，向流中输出格式化后的日志消息
    /// @tparam Args 格式化的参数的类型，与 @c std::format 格式相同
    /// @tparam condition 条件，在编译期判断
    /// @tparam stream_type 输出流的类型
    /// @param stream 输出流，接收格式化后的消息
    /// @param format 输出格式
    /// @param args 格式参数
    /// @return 消息长度，当条件不成立时返回 0
    template<bool condition, is_output_stream stream_type, typename... Args>
    std::size_t log_if(stream_type &stream, const std::string_view format, Args &&... args) {
        if constexpr (!condition) return 0;
        return log(stream, format, std::forward<Args>(args)...);
    }

    /// @brief 为日志器类启用条件日志功能
    /// @tparam Super *CRTP* 日志器父类
    template<typename Super>
    class enable_log_if {
        using super_type = Super;
        super_type &super() noexcept { return static_cast<super_type &>(*this); }

        [[nodiscard]] const super_type &super() const noexcept { return static_cast<const super_type &>(*this); }

    public:
        template<typename... Args>
        std::size_t log_if(const bool condition, const std::string_view format, Args &&... args) {
            if (!condition) return 0;
            return super().log(format, std::forward<Args>(args)...);
        }

        template<bool condition, typename... Args>
        std::size_t log_if(const std::string_view format, Args &&... args) {
            if constexpr (!condition) return 0;
            return super().log(format, std::forward<Args>(args)...);
        }
    };

    /// @brief 为类添加条件日志功能
    /// @tparam Super *CRTP* 提供了静态函数 @c log 的父类
    template<typename Super>
    struct enable_static_log_if {
        template<typename... Args>
        std::size_t log_if(const bool condition, const std::string_view format, Args &&... args) {
            if (!condition) return 0;
            return Super::log(format, std::forward<Args>(args)...);
        }

        template<bool condition, typename... Args>
        std::size_t log_if(const std::string_view format, Args &&... args) {
            if constexpr (!condition) return 0;
            return Super::log(format, std::forward<Args>(args)...);
        }
    };

    /// @brief 需要给定输出流的日志器，是否多线程安全取决于输出流是否加锁
    /// @note 标准库的 @c std::cout @c std::cerr @c std::clog 输出流是线程安全的，其他输出流不可保证
    template<is_output_stream TOStream = runtime_output_stream>
    class basic_logger : public enable_log_if<basic_logger<TOStream> > {
    public:
        using ostream_type = TOStream;
        using shared_ostream = std::shared_ptr<ostream_type>;

    protected:
        shared_ostream stream_ptr;

    public:
        explicit basic_logger(shared_ostream stream) :
            stream_ptr(std::move(stream)) {}

        template<typename... Args>
        std::size_t log(const std::string_view format, Args &&... args) {
            if (!stream_ptr) return 0;
            return logging::log(*stream_ptr, format, std::forward<Args>(args)...);
        }
    };

    /// @brief 在日志消息前添加时间戳，在消息尾附加换行符的日志器
    template<is_output_stream TOStream = runtime_output_stream>
    class cango_logger : public enable_log_if<cango_logger<TOStream> >, public basic_logger<TOStream> {
    public:
        using base_type = basic_logger<TOStream>;
        using base_type::base_type;

        template<typename... Args>
        std::size_t log(const std::string_view format, Args &&... args) {
            if (!base_type::stream_ptr) return 0;
            auto extra_format = "[{}] " + std::string(format);
            if (!extra_format.ends_with('\n')) extra_format.push_back('\n');
            const auto utc_time = std::chrono::utc_clock::now();
            return logging::log(*base_type::stream_ptr, extra_format, utc_time, std::forward<Args>(args)...);
        }
    };

    /// @brief 可能为空的日志器，当日志器为空时不输出任何消息
    template<typename TLogger>
    class maybe_logger : public enable_log_if<maybe_logger<TLogger> > {
    public:
        using logger_type = TLogger;

    private:
        std::optional<std::shared_ptr<logger_type> > logger;

    public:
        explicit maybe_logger(std::optional<std::shared_ptr<logger_type> > opt) :
            logger(std::move(opt)) {}

        template<typename... Args>
        std::size_t log(const std::string_view format, Args &&... args) {
            if (!logger.has_value() || *logger == nullptr) return 0;
            return (*logger)->log(format, std::forward<Args>(args)...);
        }
    };
}
