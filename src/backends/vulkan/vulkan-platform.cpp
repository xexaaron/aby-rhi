#include "backends/vulkan/vulkan-platform.hpp"

#include "backends/vulkan/vulkan-callbacks.hpp"
#include "context.hpp"

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
#	include <vulkan/vulkan_wayland.h>
#	include <vulkan/vulkan_xcb.h>
#	include <vulkan/vulkan_xlib.h>
#elif defined(__APPLE__)
#	include <vulkan/vulkan_macos.h>
#elif defined(__ANDROID__)
#	include <vulkan/vulkan_android.h>
#else
#	error "Unknown window backend type"
#endif

namespace aby::rhi::vulkan {

	auto create_surface(void* native_window, VkInstance instance, VkSurfaceKHR* surface) -> bool {
		auto& ctx = Context::get();
		switch (ctx.window_backend()) {
			case EWindow::win32: {
				VkWin32SurfaceCreateInfoKHR create_info{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
				create_info.pNext     = nullptr;
				create_info.hinstance = GetModuleHandle(NULL);
				create_info.hwnd      = (HWND)native_window;
				create_info.flags     = 0;
				return vkCreateWin32SurfaceKHR(instance, &create_info, allocator(), surface) == VK_SUCCESS;
			} break;
			case EWindow::x11:
				break;
			case EWindow::xcb:
				break;
			case EWindow::wayland:
				break;
			case EWindow::metal:
				break;
			default:
				break;
		};
		aby_rhi_assert(false, "unimplemented windowing backend: {}", ctx.window_backend());
		return false;
	}

	auto get_instance_extensions(std::vector<const char*>* inst_exts) -> bool {
		auto& ctx = Context::get();
#ifndef NDEBUG
		inst_exts->reserve(2);
		inst_exts->push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#else
		inst_exts->reserve(1);

#endif
		switch (ctx.window_backend()) {
#ifdef _WIN32
			case EWindow::win32:
				inst_exts->push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
				break;
#elif defined(__linux__)
			case EWindow::x11:
				inst_exts->push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
				break;
			case EWindow::xcb:
				inst_exts->push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
				break;
			case EWindow::wayland:
				inst_exts->push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
				break;
#elif defined(__APPLE__)
			case EWindow::metal:
				inst_exts->push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
				break;
#endif
			default:
				return false;
		};

		return true;
	}

} // namespace aby::rhi::vulkan
