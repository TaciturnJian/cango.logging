#include <iostream>

#include <cango/logging/std_static_log.hpp>

namespace cango::logging::details {
	std::ostream& provide_std_cout() noexcept {
		return std::cout;
	}

	std::ostream& provide_std_cerr() noexcept {
		return std::cerr;
	}

	std::ostream& provide_std_clog() noexcept {
		return std::clog;
	}
}
