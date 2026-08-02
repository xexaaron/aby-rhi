#include "renderer.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

namespace aby::rhi {

	auto Renderer::create(ERenderer backend) -> Renderer* {
		switch (backend) {
			case ERenderer::vulkan:
				return new vulkan::Renderer();
			default:
				aby_rhi_assert(false, "unimplemented renderer backend: {}", backend);
		}
		return nullptr;
	}

} // namespace aby::rhi
