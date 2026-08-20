#pragma once
#include "common-enums.hpp"
#include "common-types.hpp"

#include <chrono>

#ifndef NDEBUG
#	define aby_rhi_dbg(msg, ...) ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::debug, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
#else
#	define aby_rhi_dbg(...)
#endif
#if ABY_RHI_ENABLE_LOG_TRACE == 1
#	define aby_rhi_trc(msg, ...) ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::trace, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
#else
#	define aby_rhi_trc(...)
#endif
#if ABY_RHI_ENABLE_LOG_INFO == 1
#	define aby_rhi_log(msg, ...) ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::info, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
#else
#	define aby_rhi_log(...)
#endif
#if ABY_RHI_ENABLE_LOG_WARN == 1
#	define aby_rhi_wrn(msg, ...) ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::warn, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
#else
#	define aby_rhi_wrn(...)
#endif

#define aby_rhi_err(msg, ...) ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error, std::format(msg __VA_OPT__(, ) __VA_ARGS__))
#define aby_rhi_ftl(msg, ...) ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::fatal, std::format(msg __VA_OPT__(, ) __VA_ARGS__))

#if ABY_RHI_ENABLE_ASSERT == 1
#	if defined(_MSC_VER)
#		define ABY_RHI_DEBUG_BREAK() __debugbreak()
#	elif defined(__clang__) || defined(__GNUC__)
#		if defined(_WIN32)
#			define ABY_RHI_DEBUG_BREAK() __builtin_debugtrap()
#		elif defined(__i386__) || defined(__x86_64__)
#			define ABY_RHI_DEBUG_BREAK() __asm__ __volatile__("int3")
#		elif defined(__aarch64__) || defined(__arm__)
#			define ABY_RHI_DEBUG_BREAK() __builtin_trap()
#		else
#			define ABY_RHI_DEBUG_BREAK() __builtin_trap()
#		endif
#	else
#		include <cstdlib>
#		define ABY_RHI_DEBUG_BREAK() std::exit(2)
#	endif
#else
#	define ABY_RHI_DEBUG_BREAK()
#endif

#if defined(_MSC_VER)
#	define ABY_RHI_FUNCTION_NAME __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#	define ABY_RHI_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#	define ABY_RHI_FUNCTION_NAME __func__
#endif

#define aby_rhi_assert(expr, ...)                         \
	do {                                                  \
		if (!(expr)) {                                    \
			aby_rhi_ftl("assertion failed: {}", #expr);   \
			aby_rhi_ftl("@ {}:({})", __FILE__, __LINE__); \
			__VA_OPT__(aby_rhi_ftl(__VA_ARGS__));         \
			ABY_RHI_DEBUG_BREAK();                        \
		}                                                 \
	} while (0)

#if ABY_RHI_ENABLE_PROFILING == 1
#	define ABY_RHI_CONCAT_IMPL(x, y) x##y
#	define ABY_RHI_CONCAT(x, y) ABY_RHI_CONCAT_IMPL(x, y)
#	define ABY_RHI_STRINGIFY_IMPL(x) #x
#	define ABY_RHI_STRINGIFY(x) ABY_RHI_STRINGIFY_IMPL(x)
#	define ABY_RHI_EVAL_MACRO(x) x
#	define aby_rhi_profile(scope_name)                                                    \
		auto ABY_RHI_CONCAT(scoped_timer_, __COUNTER__) = ::aby::rhi::detail::ScopedTimer( \
		    __FILE__ ":(" ABY_RHI_STRINGIFY(ABY_RHI_EVAL_MACRO(__LINE__)) ")"              \
		                                                                  " [" scope_name "]")
#else
#	define aby_rhi_profile(...)
#endif

namespace aby::rhi::detail {

	class ScopedTimer {
	public:
		using Clock = std::chrono::steady_clock;

		explicit ScopedTimer(std::string_view name);
		~ScopedTimer();
	private:
		std::string_view m_Name;
		Clock::time_point m_Start;
	};

} // namespace aby::rhi::detail

