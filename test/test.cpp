#include <atomic>
#include <iostream>
#include <thread>

#include <cango/logging.hpp>

using namespace cango::logging;

namespace {}

int main() try {
    atomic_table<cango_logger<> > loggers;
    std::atomic_bool interrupt{false};

    std::thread ticking{
        [&loggers, &interrupt] {
            while (!interrupt) {
                std::this_thread::sleep_for(std::chrono::seconds{1});

                // segment fault
                //loggers.get("logger1")->log("Hello, world!");
                //loggers.select("logger1").log("Hello, world!");

                loggers.maybe("logger1").log("Hello world!");
            }
        }
    };

    ticking.detach();
    auto combined_stream = vcombine(
        std::make_shared<std_cout>(),
        std::make_shared<atomic_ofstream>("log.txt")
    );
    const auto logger = share_logger<cango_logger>(std::move(combined_stream));

    std::this_thread::sleep_for(std::chrono::seconds{3});
    logger->log("设置logger1日志器");
    loggers.set("logger1", logger);
    std::this_thread::sleep_for(std::chrono::seconds{3});
    logger->log("移除logger1日志器");
    loggers.remove("logger1");
    std::this_thread::sleep_for(std::chrono::seconds{3});
    logger->log("中断线程");
    interrupt = true;
    std::this_thread::sleep_for(std::chrono::seconds{2});

    return 0;
}
catch (...) { return 1; }
