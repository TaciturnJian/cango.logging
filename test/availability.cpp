#include <cango/logging.hpp>


int main() try {
    using namespace cango::logging;
    const auto cout_ostream = std::make_shared<std_cout>();
    auto cout_logger = own_logger<cango_logger>(cout_ostream);
    cout_logger->log("日志消息：{}", "Hello, World!");

    return 0;
} catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
