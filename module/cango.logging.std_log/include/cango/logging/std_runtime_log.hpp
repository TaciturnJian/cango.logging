#pragma once

#include <fstream>

#include <cango/logging/runtime_log.hpp>

#include "std_static_log.hpp"

namespace cango::logging {
	namespace details {
		template<stream_provider_ptr provider>
		struct runtime_wrapper final : runtime_output_stream {
			runtime_output_stream& output(std::string_view message) override {
				provider() << message;
				return *this;
			}
		};
	}

	using runtime_std_cout = details::runtime_wrapper<details::provide_std_cout>;
	using runtime_std_cerr = details::runtime_wrapper<details::provide_std_cerr>;
	using runtime_std_clog = details::runtime_wrapper<details::provide_std_clog>;

	struct runtime_fstream_or_cerr final : runtime_output_stream {
		std::ofstream file;

		explicit runtime_fstream_or_cerr(
			const std::filesystem::path& path, 
			std::ios::openmode mode = std::ios_base::out);

		runtime_output_stream& output(std::string_view message) override;
	};
}