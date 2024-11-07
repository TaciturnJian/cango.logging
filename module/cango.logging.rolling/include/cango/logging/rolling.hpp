#pragma once

#include <fstream>
#include <iostream>

#include <cango/logging/runtime_log.hpp>

namespace cango::logging {

	/// @brief 代表可以滚动的对象，要求使用前缀 ++ 滚动，返回自身引用
	template<typename object_type>
	concept rolling_object = requires(object_type & object) {
		{ ++object } -> std::same_as<object_type&>;
	};

	/// @brief 滚动的文件名，格式为 <prefix><id>.log
	struct rolling_filename {
		std::string prefix;
		std::uint64_t id{ 0 };

		[[nodiscard]] std::string get_filename() const {
			return std::format("{}{}.log", prefix, id);
		}

		rolling_filename& operator++() {
			++id;
			return *this;
		}
	};

	/// @brief 使用 @c rolling_filename 的滚动文件流，当文件大小超过 10MB 时滚动
	/// @warning 在文件流不可用时使用 std::cerr 输出
	class runtime_rolling_fstream_or_cerr final : public runtime_output_stream {
	public:
		static constexpr auto kb = 1024;
		static constexpr auto mb = 1024 * kb;
		static constexpr std::size_t max_size = 10 * mb;

		rolling_filename filename;

		explicit runtime_rolling_fstream_or_cerr(std::string prefix) : filename{ std::move(prefix) } {}

	private:
		std::size_t current_size{0};
		std::shared_ptr<std::ofstream> stream{};

	public:
		runtime_output_stream& output(std::string_view message) override {
			if (!stream || current_size >= max_size) {
				++filename;
				current_size = 0;
				stream = std::make_shared<std::ofstream>(filename.get_filename());
			}

			if (!stream->is_open() || stream->bad()) 
				std::cerr << message;
			else {			
				*stream << message;
				current_size += message.size();
			}

			return *this;
		}
	};
}