#include "texture.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-texture.hpp"
#include "context.hpp"
namespace aby::rhi {

	auto Texture::create(const fs::path& rel_path, const TextureParams& params) -> ResourcePtr<Texture, EResource::texture> {
		auto backend = Context::get().renderer_backend();
		auto* jobs   = Context::get().job_sys();
		auto& texs   = Context::get().textures();
		switch (backend) {
			case ERenderer::vulkan: {
				Resource resource = texs.reserve();

				jobs->add_job(EJobPriority::high, [rel_path, resource, params]() {
					auto& texs = Context::get().textures();
					auto tex   = new vulkan::Texture(rel_path, params);
					auto* r    = static_cast<vulkan::Renderer*>(Context::get().renderer());

					r->gc().push([p = tex] {
						if (p) {
							p->destroy();
						}
					});

					texs.add(resource, tex);
				});

				return ResourcePtr<Texture, EResource::texture>(resource.id(), &Context::get().textures());
			}
			default:
				aby_rhi_assert(false, "unimplemented renderer backend");
		}
		return ResourcePtr<Texture, EResource::texture>();
	}

} // namespace aby::rhi
