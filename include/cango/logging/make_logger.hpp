#pragma once

#include "logger.hpp"

namespace cango::logging {
    /// @brief 创建日志器，给出指向日志器的 @c std::unique_ptr
    template<template<typename T> typename TLogger, is_output_stream TOStream>
    std::unique_ptr<TLogger<TOStream> > own_logger(std::shared_ptr<TOStream> stream) {
        return std::make_unique<TLogger<TOStream> >(std::move(stream));
    }

    /// @brief 创建日志器，给出指向日志器的 @c std::shared_ptr
    template<template<typename T> typename TLogger, is_output_stream TOStream>
    std::shared_ptr<TLogger<TOStream> > share_logger(std::shared_ptr<TOStream> stream) {
        return std::make_shared<TLogger<TOStream> >(std::move(stream));
    }

    /// @brief 创建一个由 @c maybe_logger 包装的 @c TLogger 日志器
    template<template<typename T> typename TLogger, is_output_stream TOStream>
    maybe_logger<TLogger<TOStream> > make_maybe(std::shared_ptr<TOStream> stream) {
        return maybe_logger<TLogger<TOStream> >{std::make_optional(share_logger<TLogger, TOStream>(std::move(stream)))};
    }
}
