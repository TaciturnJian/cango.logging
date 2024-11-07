#pragma once

#include <memory>
#include <mutex>

#include "static_log.hpp"

namespace cango::logging {
	/// @brief 在运行时确定的输出流
	struct runtime_output_stream {
		virtual ~runtime_output_stream() = default;

		/// @brief 输出消息
		/// @param message 要输出的消息，必须确保在输出期间存在
		/// @return 当前对象
		virtual runtime_output_stream& output(std::string_view message) {
			throw std::runtime_error("not implemented");
		}

		/// @brief 使用 @c output 函数输出消息
		/// @param message 要输出的消息，必须确保在输出期间存在
		/// @return 当前对象
		runtime_output_stream& operator<<(const std::string_view message) {
			return output(message);
		}
	};
	static_assert(output_stream<runtime_output_stream>, "runtime_stream must be output_stream");

	/// @brief 为日志器类启用条件日志功能
	/// @tparam Super *CRTP* 日志器父类
	template<typename Super>
	class enable_log_if {
		using super_type = Super;
		super_type& super() noexcept {
			return static_cast<super_type&>(*this);
		}

		[[nodiscard]] const super_type& super() const noexcept {
			return static_cast<const super_type&>(*this);
		}

	public:
		template<typename ...Args>
		std::size_t log_if(const bool condition, const std::string_view format, Args&&... args) {
			if (!condition) return 0;
			return super().log(format, std::forward<Args>(args)...);
		}

		template<bool condition, typename ...Args>
		std::size_t log_if(const std::string_view format, Args&&... args) {
			if constexpr (!condition) return 0;
			return super().log(format, std::forward<Args>(args)...);
		}
	};

	/// @brief 运行期确定输出流的日志器，不可用于多线程
	class basic_logger : public enable_log_if<basic_logger> {
		std::shared_ptr<runtime_output_stream> stream_ptr;

	public:
		explicit basic_logger(std::shared_ptr<runtime_output_stream> stream) :
			stream_ptr(std::move(stream)) {
		}

		template<typename ...Args>
		std::size_t log(const std::string_view format, Args&&... args) {
			if (!stream_ptr) return 0;
			return cango::logging::log(*stream_ptr, format, std::forward<Args>(args)...);
		}
	};

	/// @brief 运行期确定输出流的日志器，多线程安全
	class atomic_logger : public enable_log_if<atomic_logger> {
		std::shared_ptr<runtime_output_stream> stream_ptr;
		std::mutex lock;

	public:
		explicit atomic_logger(std::shared_ptr<runtime_output_stream> stream) :
			stream_ptr(std::move(stream)) {
		}

		template<typename ...Args>
		std::size_t log(const std::string_view format, Args&&... args) {
			std::lock_guard guard{ lock };
			if (!stream_ptr) return 0;
			return cango::logging::log(*stream_ptr, format, std::forward<Args>(args)...);
		}
	};
}
