#pragma once

#include <mutex>

#include "output_stream.hpp"

namespace cango::logging {
    /// @brief 代表类型未被擦除的输出流，因为不继承 @c runtime_output_stream ，所以不可用于运行时多态
    template<typename object_type>
    concept is_not_type_erased_output_stream =
        is_output_stream<object_type> && !std::derived_from<object_type, runtime_output_stream>;

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
    /// @tparam TImpl 输出流类型
    /// @note 如果你尝试使用此类封装 @c runtime_output_stream 类型的输出流，将会导致编译错误
    template<is_not_type_erased_output_stream TImpl>
    class wrap_output_stream<TImpl, wrap_type::erase_type> final : public runtime_output_stream, TImpl {
    public:
        /// @brief 原输出流类型
        using impl_type = TImpl;

        /// @brief 继承基类的构造函数
        using impl_type::impl_type;

        /// @brief 输出消息，返回流引用
        void output(std::string_view message) override { static_cast<impl_type *>(this)->output(message); }
    };

    /// @brief 封装符合条件的输出流，使其线程安全
    /// @tparam TImpl 输出流类型
    /// @note 如果你尝试使用此类封装 @c runtime_output_stream 类型的输出流，将会导致编译错误
    template<is_not_type_erased_output_stream TImpl>
    class wrap_output_stream<TImpl, wrap_type::make_atomic> final : TImpl {
    public:
        using impl_type = TImpl;

    private:
        std::mutex mutex;

    public:
        using impl_type::impl_type;

        void output(std::string_view message) {
            std::lock_guard lock(mutex);
            static_cast<impl_type *>(this)->output(message);
        }
    };
}
