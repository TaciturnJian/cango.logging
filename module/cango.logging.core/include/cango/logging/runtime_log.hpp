#pragma once

#include <memory>

#include "static_log.hpp"

namespace cango::logging {
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

    /// @brief 需要给定输出流的日志器，是否多线程安全取决于输出流是否加锁
    /// @note 标准库的 @c std::cout @c std::cerr @c std::clog 输出流是线程安全的，其他输出流不可保证
    template<is_output_stream TOStream = runtime_output_stream>
    class basic_logger : public enable_log_if<basic_logger<TOStream> > {
    public:
        using ostream_type = TOStream;
        using shared_ostream = std::shared_ptr<ostream_type>;

    private:
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
}
