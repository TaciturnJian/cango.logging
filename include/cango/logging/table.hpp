#pragma once

#include <unordered_map>

#include "logger.hpp"

namespace cango::logging {
    template<typename TLogger = basic_logger>
    class table {
    public:
        using logger_type = TLogger;

    private:
        std::unordered_map<std::string, std::shared_ptr<logger_type> > loggers;

    public:
        [[nodiscard]] std::shared_ptr<logger_type> get(const std::string &name) const {
            if (!loggers.contains(name)) return nullptr;
            return loggers.at(name);
        }

        void set(const std::string &name, std::shared_ptr<logger_type> logger) { loggers[name] = logger; }

        void remove(const std::string &name) { loggers.erase(name); }

        logger_type &select(const std::string &name) { return *loggers.at(name); }

        maybe_logger<logger_type> maybe(const std::string &name) {
            if (!loggers.contains(name)) return maybe_logger<logger_type>{std::nullopt};

            return maybe_logger<logger_type>{loggers.at(name)};
        }
    };

    template<typename logger_type = basic_logger>
    class atomic_table : table<logger_type> {
        std::mutex locker;

    public:
        std::shared_ptr<logger_type> get(const std::string &name) {
            std::lock_guard lock{locker};
            return table<logger_type>::get(name);
        }

        void set(const std::string &name, std::shared_ptr<logger_type> logger) {
            std::lock_guard lock{locker};
            return table<logger_type>::set(name, std::move(logger));
        }

        void remove(const std::string &name) {
            std::lock_guard lock{locker};
            return table<logger_type>::remove(name);
        }

        logger_type &select(const std::string &name) {
            std::lock_guard lock{locker};
            return table<logger_type>::select(name);
        }

        maybe_logger<logger_type> maybe(const std::string &name) {
            std::lock_guard lock{locker};
            return table<logger_type>::maybe(name);
        }
    };
}
