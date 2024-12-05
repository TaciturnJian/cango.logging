#pragma once

#include <concepts>
#include <mutex>
#include <ostream>
#include <string_view>

namespace cango::logging {
    /// @brief 代表可以输出一系列字节的对象，简称为输出流，日志器会将格式化的消息输出到流中
    template<typename object_type>
    concept is_output_stream = requires(object_type &object, std::string_view message) {
        { object << message };
    };

    /// @brief 在运行时确定的输出流
    struct runtime_output_stream {
        virtual ~runtime_output_stream() = default;

        /// @brief 输出消息
        /// @param message 要输出的消息，必须确保在输出期间存在
        /// @return 当前对象
        virtual runtime_output_stream &output(std::string_view message) { throw std::runtime_error("not implemented"); }

        /// @brief 使用 @c output 函数输出消息
        /// @param message 要输出的消息，必须确保在输出期间存在
        /// @return 当前对象
        runtime_output_stream &operator<<(const std::string_view message) { return output(message); }
    };

    /// @brief 代表类型未被擦除的输出流，因为不继承 @c runtime_output_stream ，所以不可用于运行时多态
    template<typename object_type>
    concept is_not_type_erased_output_stream = is_output_stream<object_type> && !std::derived_from<object_type,
                                                   runtime_output_stream>;

    /// @brief 封装类型
    enum class wrap_type {
        /// @brief 擦除类型，用于运行时多态
        erase_type,

        /// @brief 将写入操作原子化，用于多线程环境
        make_atomic
    };

    template<is_output_stream TImpl, wrap_type TWrap>
    class wrap_output_stream;

    /// @brief 封装符合条件的输出流，擦去其类型以在运行时确定，辅助实现运行时多态
    /// @tparam TImpl 输出流类型，实现了 @c is_output_stream 接口
    /// @note 如果你尝试使用此类封装 @c runtime_output_stream 类型的输出流，将会导致编译错误
    template<is_not_type_erased_output_stream TImpl>
    class wrap_output_stream<TImpl, wrap_type::erase_type> final : public runtime_output_stream, TImpl {
    public:
        /// @brief 未封装的输出流类型
        using impl_type = TImpl;

        /// @brief 继承基类的构造函数
        using impl_type::impl_type;

        /// @brief 输出消息，返回流引用
        runtime_output_stream &output(std::string_view message) override {
            *static_cast<impl_type *>(this) << message;
            return *static_cast<runtime_output_stream *>(this);
        }
    };

    /// @brief 封装符合条件的输出流，使其线程安全
    template<is_not_type_erased_output_stream TImpl>
    class wrap_output_stream<TImpl, wrap_type::make_atomic> final : TImpl {
    public:
        using impl_type = TImpl;

    private:
        std::mutex mutex;

    public:
        using impl_type::impl_type;

        wrap_output_stream &operator<<(const std::string_view message) {
            std::lock_guard lock(mutex);
            *static_cast<impl_type *>(this) << message;
            return *this;
        }
    };

    namespace details::code_guarantee {
        static_assert(is_output_stream<runtime_output_stream>, "runtime_stream must be output_stream");
    }
}
