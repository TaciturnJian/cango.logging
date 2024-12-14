#pragma once

#include "details/combined_output_stream.hpp"

namespace cango::logging {
    template<is_output_stream ...TOStreamList>
    std::shared_ptr<details::combined_output_stream<TOStreamList...> >
    combine(std::shared_ptr<TOStreamList>... streams) {
        return std::make_shared<details::combined_output_stream<TOStreamList...> >(std::move(streams)...);
    }

    template<is_output_stream ...TOStreamList>
    std::shared_ptr<runtime_output_stream>
    vcombine(std::shared_ptr<TOStreamList>... streams) {
        return std::make_shared<details::type_erased_combined_output_stream<TOStreamList...> >(std::move(streams)...);
    }
}
