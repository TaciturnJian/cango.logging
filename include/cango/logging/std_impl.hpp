#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "wrap.hpp"

#include "details/wrap_ostream.hpp"

namespace cango::logging {
    using std_cout = details::wrap_static_ostream<&std::cout>;
    using std_cerr = details::wrap_static_ostream<&std::cerr>;
    using std_clog = details::wrap_static_ostream<&std::clog>;
    using std_ofstream = details::wrap_ostream<std::ofstream>;
    using std_osstream = details::wrap_ostream<std::ostringstream>;
    using atomic_ofstream = wrap_output_stream<std_ofstream, wrap_type::make_atomic>;
    using atomic_osstream = wrap_output_stream<std_osstream, wrap_type::make_atomic>;
    using runtime_std_cout = wrap_output_stream<std_cout, wrap_type::erase_type>;
    using runtime_std_cerr = wrap_output_stream<std_cerr, wrap_type::erase_type>;
    using runtime_std_clog = wrap_output_stream<std_clog, wrap_type::erase_type>;
    using runtime_std_ofstream = wrap_output_stream<std_ofstream, wrap_type::erase_type>;
    using runtime_atomic_ofstream = wrap_output_stream<atomic_ofstream, wrap_type::erase_type>;
    using runtime_atomic_osstream = wrap_output_stream<atomic_osstream, wrap_type::erase_type>;
}
