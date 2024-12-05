#pragma once

#include <fstream>
#include <iostream>

#include <cango/logging/runtime_log.hpp>
#include <cango/logging/static_log.hpp>

namespace cango::logging {
    namespace details {
        template<std::ostream * stream_ref>
        struct wrap_std_ostream {
            std::ostream &operator<<(const std::string_view message) const { return *stream_ref << message; }
        };
    }

    using std_cout = details::wrap_std_ostream<&std::cout>;
    using std_cerr = details::wrap_std_ostream<&std::cerr>;
    using std_clog = details::wrap_std_ostream<&std::clog>;
    using std_ofstream = std::ofstream;
    using atomic_ofstream = wrap_output_stream<std_ofstream, wrap_type::make_atomic>;
    using runtime_std_cout = wrap_output_stream<std_cout, wrap_type::erase_type>;
    using runtime_std_cerr = wrap_output_stream<std_cerr, wrap_type::erase_type>;
    using runtime_std_clog = wrap_output_stream<std_clog, wrap_type::erase_type>;
    using runtime_std_ofstream = wrap_output_stream<std_ofstream, wrap_type::erase_type>;
    using runtime_atomic_ofstream = wrap_output_stream<atomic_ofstream, wrap_type::erase_type>;
    using std_cout_logger = basic_logger<std_cout>;
    using std_cerr_logger = basic_logger<std_cerr>;
    using std_clog_logger = basic_logger<std_clog>;
    using std_ofstream_logger = basic_logger<std_ofstream>;
    using atomic_ofstream_logger = basic_logger<atomic_ofstream>;
}
