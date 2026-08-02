#pragma once
#include "common-enums.hpp"
#include "common-types.hpp"

#include <format>

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
#		define ABY_RHI_DEBUG_BREAK() __debug_break()
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

#define aby_rhi_assert(expr, ...)                         \
	do {                                                  \
		if (!(expr)) {                                    \
			aby_rhi_ftl("assertion failed: {}", #expr);   \
			aby_rhi_ftl("@ {}:({})", __FILE__, __LINE__); \
			__VA_OPT__(aby_rhi_ftl(__VA_ARGS__));         \
			ABY_RHI_DEBUG_BREAK();                        \
		}                                                 \
	} while (0)

/**
 * @brief Formatters for common types and enums. 
 */
namespace std {

	template <>
	struct formatter<aby::rhi::EAllocation, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::EAllocation value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::EAllocation::backend:
					str = "backend";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::EShader, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::EShader value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::EShader::none:
					str = "none";
					break;
				case aby::rhi::EShader::vert:
					str = "vert";
					break;
				case aby::rhi::EShader::frag:
					str = "frag";
					break;
				case aby::rhi::EShader::comp:
					str = "comp";
					break;
				case aby::rhi::EShader::geom:
					str = "geom";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::ELogLevel, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::ELogLevel value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::ELogLevel::debug:
					str = "debug";
					break;
				case aby::rhi::ELogLevel::trace:
					str = "trace";
					break;
				case aby::rhi::ELogLevel::info:
					str = "info";
					break;
				case aby::rhi::ELogLevel::warn:
					str = "warn";
					break;
				case aby::rhi::ELogLevel::error:
					str = "error";
					break;
				case aby::rhi::ELogLevel::fatal:
					str = "fatal";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::ERenderer, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::ERenderer value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::ERenderer::vulkan:
					str = "vulkan";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::EWindow, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::EWindow value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::EWindow::win32:
					str = "win32";
					break;
				case aby::rhi::EWindow::x11:
					str = "x11";
					break;
				case aby::rhi::EWindow::xcb:
					str = "xcb";
					break;
				case aby::rhi::EWindow::wayland:
					str = "wayland";
					break;
				case aby::rhi::EWindow::metal:
					str = "metal";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::ETopology, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::ETopology value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::ETopology::point_list:
					str = "point_list";
					break;
				case aby::rhi::ETopology::line_list:
					str = "line_list";
					break;
				case aby::rhi::ETopology::line_strip:
					str = "line_strip";
					break;
				case aby::rhi::ETopology::triangle_list:
					str = "triangle_list";
					break;
				case aby::rhi::ETopology::triangle_strip:
					str = "triangle_strip";
					break;
				case aby::rhi::ETopology::triangle_fan:
					str = "triangle_fan";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::EPolygonMode, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::EPolygonMode value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::EPolygonMode::fill:
					str = "fill";
					break;
				case aby::rhi::EPolygonMode::line:
					str = "line";
					break;
				case aby::rhi::EPolygonMode::point:
					str = "point";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::ECullMode, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::ECullMode value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::ECullMode::none:
					str = "none";
					break;
				case aby::rhi::ECullMode::front:
					str = "front";
					break;
				case aby::rhi::ECullMode::back:
					str = "back";
					break;
				case aby::rhi::ECullMode::front_and_back:
					str = "front_and_back";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::EFrontFace, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::EFrontFace value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::EFrontFace::clockwise:
					str = "clockwise";
					break;
				case aby::rhi::EFrontFace::counter_clockwise:
					str = "counter_clockwise";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::rhi::EFormat, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::rhi::EFormat value, FmtContext& ctx) const {
			std::string_view str = "unknown";

			switch (value) {
				case aby::rhi::EFormat::none:
					str = "none";
					break;
				case aby::rhi::EFormat::rgba_f32:
					str = "float[4]";
					break;
				case aby::rhi::EFormat::rgb_f32:
					str = "float[3]";
					break;
				case aby::rhi::EFormat::rg_f32:
					str = "float[2]";
					break;
				case aby::rhi::EFormat::r_f32:
					str = "float";
					break;
				case aby::rhi::EFormat::rgba_i32:
					str = "int32[4]";
					break;
				case aby::rhi::EFormat::rgb_i32:
					str = "int32[3]";
					break;
				case aby::rhi::EFormat::rg_i32:
					str = "int32[2]";
					break;
				case aby::rhi::EFormat::r_i32:
					str = "int32";
					break;
				case aby::rhi::EFormat::rgba_u32:
					str = "uint32[4]";
					break;
				case aby::rhi::EFormat::rgb_u32:
					str = "uint32[3]";
					break;
				case aby::rhi::EFormat::rg_u32:
					str = "uint32[2]";
					break;
				case aby::rhi::EFormat::r_u32:
					str = "uint32";
					break;
			}

			return std::ranges::copy(str, ctx.out()).out;
		}
	};

} // namespace std
