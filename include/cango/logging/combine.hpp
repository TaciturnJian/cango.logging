#pragma once

#include "wrap.hpp"

#include "details/combined_output_stream.hpp"

namespace cango::logging {
    /// @brief 将提供的多个流结合成一个流，这个流会将输入按传参顺序依次转发到所有的子流中
    /// @tparam TOStreamList 输出流的类型列表，要求所有类型都符合 @c is_output_stream 概念
    /// @param streams 要结合的输出流，需要使用共享指针，使用移动构造可以避免性能损失
    /// @return 指向结合流的共享指针
    template<is_output_stream ...TOStreamList>
    std::shared_ptr<details::combined_output_stream<TOStreamList...> >
    combine(std::shared_ptr<TOStreamList>... streams) {
        return std::make_shared<details::combined_output_stream<TOStreamList...> >(std::move(streams)...);
    }

    /// @brief 将提供的多个流结合成一个流，这个流会将输入按传参顺序依次转发到所有的子流中
    /// @return 指向类型被擦除的结合流的共享指针
    template<is_output_stream ...TOStreamList>
    std::shared_ptr<runtime_output_stream>
    vcombine(std::shared_ptr<TOStreamList>... streams) {
        using stream_type = wrap_output_stream<details::combined_output_stream<TOStreamList...>, wrap_type::erase_type>;
        return std::make_shared<stream_type>(std::move(streams)...);
    }
}
