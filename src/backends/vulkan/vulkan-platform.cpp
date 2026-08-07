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
#ifdef _WIN32
			case EWindow::win32: {
				VkWin32SurfaceCreateInfoKHR create_info{
					VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR
				};
				create_info.hinstance = GetModuleHandle(NULL);
				create_info.hwnd      = static_cast<HWND>(native_window);
				return vkCreateWin32SurfaceKHR(
				           instance,
				           &create_info,
				           allocator(),
				           surface) == VK_SUCCESS;
			}
#elif defined(__linux__)
			case EWindow::x11: {
				auto& [display, window] = *static_cast<std::pair<Display*, Window>*>(native_window);
				VkXlibSurfaceCreateInfoKHR create_info{
					VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR
				};
				create_info.dpy    = display;
				create_info.window = window;
				return vkCreateXlibSurfaceKHR(
				           instance,
				           &create_info,
				           allocator(),
				           surface) == VK_SUCCESS;
			}
			case EWindow::xcb: {
				auto& [connection, window] = *static_cast<std::pair<xcb_connection_t*, xcb_window_t>*>(native_window);
				VkXcbSurfaceCreateInfoKHR create_info{
					VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR
				};
				create_info.connection = connection;
				create_info.window     = window;
				return vkCreateXcbSurfaceKHR(
				           instance,
				           &create_info,
				           allocator(),
				           surface) == VK_SUCCESS;
			}
			case EWindow::wayland: {
				auto& [display, wl_surface] = *static_cast<std::pair<wl_display*, wl_surface*>*>(native_window);
				VkWaylandSurfaceCreateInfoKHR create_info{
					VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR
				};
				create_info.display = display;
				create_info.surface = wl_surface;
				return vkCreateWaylandSurfaceKHR(
				           instance,
				           &create_info,
				           allocator(),
				           surface) == VK_SUCCESS;
			}
#elif defined(__APPLE__)
			case EWindow::metal: {
				VkMetalSurfaceCreateInfoEXT create_info{
					VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT
				};
				create_info.pLayer = static_cast<CAMetalLayer*>(native_window);
				return vkCreateMetalSurfaceEXT(
				           instance,
				           &create_info,
				           allocator(),
				           surface) == VK_SUCCESS;
			}
#endif
			default:
				break;
		}
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

	auto get_device_extensions() -> std::vector<const char*> {
		std::vector<const char*> dev_exts;
		dev_exts.reserve(5);
		dev_exts.push_back(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
		dev_exts.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
		dev_exts.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
		dev_exts.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
		dev_exts.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		return dev_exts;
	}

	auto get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool {
		*dev_exts = get_device_extensions();
		return get_instance_extensions(inst_exts);
	}

} // namespace aby::rhi::vulkan
