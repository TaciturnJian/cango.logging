#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>

#include "logger.hpp"
#include "wrap.hpp"

namespace cango::logging {
    /// @brief 使用 @c rolling_filename 的滚动文件流，当文件大小超过 10MB 时滚动
    /// @warning 在文件流不可用时使用 std::cerr 输出
    class rolling_ofstream {
    public:
        static constexpr auto kb = 1024;
        static constexpr auto mb = 1024 * kb;
        static constexpr std::size_t max_size = 10 * mb;
        using file_path_provider = std::function<std::filesystem::path()>;

    private:
        file_path_provider path_provider;
        std::filesystem::path current_file{};
        std::size_t current_size{0};
        std::shared_ptr<std::ofstream> stream{};

    public:
        explicit rolling_ofstream(file_path_provider pathProvider) noexcept :
            path_provider(std::move(pathProvider)) {}

        [[nodiscard]] bool is_open() const noexcept { return stream != nullptr && stream->is_open(); }

        void output(const std::string_view message) {
            if (stream == nullptr || current_size >= max_size) {
                current_file = path_provider();
                current_size = 0;
                stream = std::make_shared<std::ofstream>(current_file);
            }

            if (stream->is_open() && !stream->bad()) {
                *stream << message;
                current_size += message.size();
            }
            else std::cerr << message;
        }
    };

    using runtime_rolling_ofstream = wrap_output_stream<rolling_ofstream, wrap_type::erase_type>;
    using atomic_rolling_ofstream = wrap_output_stream<rolling_ofstream, wrap_type::make_atomic>;
    using runtime_atomic_rolling_ofstream = wrap_output_stream<atomic_rolling_ofstream, wrap_type::erase_type>;
}
