#include "backends/vulkan/vulkan-callbacks.hpp"

#include "context.hpp"

namespace aby::rhi::vulkan {

	auto allocator() -> VkAllocationCallbacks* {
		static VkAllocationCallbacks alloc_cbs;

		if (!alloc_cbs.pfnFree) { // lazy check only one, if one is missing then we create.
			alloc_cbs.pUserData       = Context::get().allocator();
			alloc_cbs.pfnAllocation   = vk_alloc_callback;
			alloc_cbs.pfnReallocation = vk_realloc_callback;
			alloc_cbs.pfnFree         = vk_free_callback;
		}

		return &alloc_cbs;
	}

	auto VKAPI_PTR vk_alloc_callback(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) -> void* {
		auto* allocator = (IAllocator*)pUserData;
		return allocator->alloc(size, alignment, EAllocation::backend);
	}

	auto VKAPI_PTR vk_realloc_callback(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) -> void* {
		auto* allocator = (IAllocator*)pUserData;
		return allocator->realloc(pOriginal, size, alignment, EAllocation::backend);
	}

	auto VKAPI_PTR vk_free_callback(void* pUserData, void* pMemory) -> void {
		auto* allocator = (IAllocator*)pUserData;
		allocator->free(pMemory, EAllocation::backend);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
	    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	    VkDebugUtilsMessageTypeFlagsEXT type,
	    const VkDebugUtilsMessengerCallbackDataEXT* data,
	    void* user_data) {
		auto log = Context::get().logger();
		switch (severity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				log->log(ELogLevel::error, data->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
				std::string msg(data->pMessage);

				// layer name X does not conform to naming standard
				if (msg.contains("Policy #LLP_LAYER_3")) {
					break;
				}

				// Older obs hook
				if (msg.contains("API version 1.3 which is older")) {
					break;
				}

				log->log(ELogLevel::warn, data->pMessage);

				break;
			}
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
				std::string msg(data->pMessage);
				if (msg.contains("debugPrintf")) {
					log->log(ELogLevel::info, data->pMessage);
				}
				break;
			}
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				log->log(ELogLevel::trace, data->pMessage);
				break;
		}
		return VK_SUCCESS;
	}

} // namespace aby::rhi::vulkan
