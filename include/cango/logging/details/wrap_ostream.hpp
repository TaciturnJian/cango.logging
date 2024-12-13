#pragma once

#include <ostream>

namespace cango::logging::details {
    template<std::ostream * TPtr>
    struct wrap_static_ostream final : enable_log_if<wrap_static_ostream<TPtr> > {
        void output(const std::string_view message) { *TPtr << message; }

        template<typename... Args>
        static std::size_t log(const std::string_view format, Args &&... args) {
            wrap_static_ostream stream{};
            return logging::log(stream, format, std::forward<Args>(args)...);
        }
    };

    template<typename TOStream>
        requires std::is_base_of_v<std::ostream, TOStream>
    struct wrap_ostream : TOStream {
        using impl_type = TOStream;
        using impl_type::impl_type;
        void output(const std::string_view message) { *static_cast<impl_type *>(this) << message; }
    };
}
