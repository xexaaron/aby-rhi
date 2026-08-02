#include "texture.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-texture.hpp"

namespace aby::rhi {

	auto Texture::create(const fs::path& rel_path) -> std::shared_ptr<Texture> {
		auto backend = Context::get().renderer_backend();
		switch (backend) {
			case ERenderer::vulkan: {
				auto tex = std::make_shared<vulkan::Texture>(rel_path);
				auto* r  = static_cast<vulkan::Renderer*>(Context::get().renderer());
				r->gc().push([weak = std::weak_ptr(tex)] {
					if (auto p = weak.lock())
						p->destroy();
				});
				return tex;
			}
			default:
				aby_rhi_assert(false, "unimplemented renderer backend");
		}
		return nullptr;
	}

} // namespace aby::rhi
