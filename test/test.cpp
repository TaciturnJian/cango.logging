#include <iostream>

#include <cango/logging.hpp>

using namespace cango::logging;

namespace {}

int main() try {
    auto logger = make_maybe<cango_logger>(std::make_shared<std_cout>());
    logger.log("hello world!");

    return 0;
}
catch (...) { return 1; }
