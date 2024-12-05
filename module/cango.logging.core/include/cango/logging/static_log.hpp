#pragma once

#include <cstddef>
#include <format>
#include <string_view>

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
        const auto message = std::vformat(format, std::make_format_args(args...));
        stream << message;
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
}
