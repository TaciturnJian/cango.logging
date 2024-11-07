#pragma once

#include <cango/logging/static_log.hpp>

namespace cango::logging {
	namespace details {
		using stream_provider_ptr = std::ostream& (*)();

		template<stream_provider_ptr provider>
		class generate_static_stream_log {
		public:
			template<typename ...Args>
			static std::size_t log(const std::string_view format, Args&&... args) {
				auto& stream = provider();
				return cango::logging::log(stream, format, std::forward<Args>(args)...);
			}

			template<typename ...Args>
			static std::size_t log_if(bool condition, const std::string_view format, Args&&... args) {
				if (!condition) return 0;
				return log(format, std::forward<Args>(args)...);
			}

			template<bool condition, typename ...Args>
			static std::size_t log_if(const std::string_view format, Args&&... args) {
				if constexpr (!condition) return 0;
				return log(format, std::forward<Args>(args)...);
			}
		};

		std::ostream& provide_std_cout() noexcept;
		std::ostream& provide_std_cerr() noexcept;
		std::ostream& provide_std_clog() noexcept;
	}

	using std_cout = details::generate_static_stream_log<details::provide_std_cout>;
	using std_cerr = details::generate_static_stream_log<details::provide_std_cerr>;
	using std_clog = details::generate_static_stream_log<details::provide_std_clog>;
}
