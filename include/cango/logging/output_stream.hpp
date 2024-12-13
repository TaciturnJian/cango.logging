#pragma once

#include <concepts>
#include <string_view>

namespace cango::logging {
    /// @brief 代表可以输出一系列字节的对象，简称为输出流，日志器会将格式化的消息输出到流中
    template<typename object_type>
    concept is_output_stream = requires(object_type &object, std::string_view message) {
        { object.output(message) };
    };

    /// @brief 在运行时确定的输出流
    struct runtime_output_stream {
        virtual ~runtime_output_stream() = default;

        /// @brief 输出消息
        /// @param message 要输出的消息，必须确保在输出期间存在
        /// @return 当前对象
        virtual void output(std::string_view message) { throw std::runtime_error("not implemented"); }
    };
}
