#include <iostream>
#include <filesystem>

#include <cango/logging/std_runtime_log.hpp>

namespace cango::logging {
	runtime_fstream_or_cerr::runtime_fstream_or_cerr(
		const std::filesystem::path& path,
		const std::ios::openmode mode) {
		file.open(path, mode);
		if (!file.is_open())
			std::cerr << std::format("无法打开日志文件（{}），输出将被重定向至(std::cerr)\n", path.string());
	}

	runtime_output_stream& runtime_fstream_or_cerr::output(std::string_view message) {
		if (file.is_open() && !file.bad())
			file << message;
		else
			std::cerr << message;

		return *this;
	}
}
