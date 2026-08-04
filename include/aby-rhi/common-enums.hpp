#pragma once
#include <cstdint>
namespace aby::rhi {

	enum class EAllocation {
		backend,
	};

	enum class ELogLevel {
		debug,
		trace,
		info,
		warn,
		error,
		fatal,
	};

	enum class ERenderer {
		vulkan,
	};

	enum class EWindow {
		win32,
		x11,
		xcb,
		wayland,
		metal
	};

	enum class EShader {
		none = 0,
		vert = 1,
		frag = 2,
		px   = frag,
		comp = 3,
		geom = 4,
	};

	enum class ETopology {
		point_list,
		line_list,
		line_strip,
		triangle_list,
		triangle_strip,
		triangle_fan,
	};

	enum class EPolygonMode {
		fill,
		line,
		point,
	};

	enum class ECullMode {
		none,
		front,
		back,
		front_and_back,
	};

	enum class EFrontFace {
		clockwise,
		counter_clockwise,
	};

	enum class EFormat {
		none,

		rgba_f32,
		rgb_f32,
		rg_f32,
		r_f32,

		rgba_f16,
		rgb_f16,
		rg_f16,
		r_f16,

		rgba_i32,
		rgb_i32,
		rg_i32,
		r_i32,

		rgba_u32,
		rgb_u32,
		rg_u32,
		r_u32,
	};

	enum class EJobPriority {
		low      = 0,
		medium   = 1,
		high     = 2,
		critical = 3
	};

	enum class EJobQueue {
		textures = 0,
		shaders  = 1,
		caching  = 2,
		max_queues
	};

	enum class EResource : uint32_t {
		none,
		texture,
		shader,
	};

	enum class EResourceState : uint16_t {
		invalid,
		loading,
		loaded,
		failed,
	};

	enum class ResourceID : uint32_t {
		invalid = UINT32_MAX,
	};

} // namespace aby::rhi
