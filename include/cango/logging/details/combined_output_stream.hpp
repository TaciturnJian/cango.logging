#pragma once

#include <memory>
#include <tuple>

#include "../output_stream.hpp"

namespace cango::logging::details {
    template<is_output_stream ...TOStreamList>
    class combined_output_stream final {
        std::tuple<std::shared_ptr<TOStreamList>...> stream_ptr_list;

    public:
        explicit combined_output_stream(std::shared_ptr<TOStreamList>... streams) :
            stream_ptr_list(std::move(streams)...) {}

        void output(std::string_view message) {
            std::apply([&message](auto &... streams) { ((streams->output(message)), ...); }, stream_ptr_list);
        }
    };
}
