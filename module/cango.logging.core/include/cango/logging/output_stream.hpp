#pragma once

#include <concepts>
#include <ostream>
#include <string_view>

namespace cango::logging {
	/// @brief 代表可以输出连续字节的流
	template<typename object_type>
	concept output_stream = requires(object_type & object, std::string_view message) {
		{ object << message } -> std::same_as<object_type&>;
	};
}
