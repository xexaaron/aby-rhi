#include "texture.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-texture.hpp"
#include "context.hpp"
namespace aby::rhi {

	auto Texture::create(const fs::path& rel_path, const TextureParams& params) -> ResourcePtr<Texture, EResource::texture> {
		aby_rhi_assert(Context::get().job_sys(), "context was not initialized");
		auto backend = Context::get().renderer_backend();
		auto* jobs   = Context::get().job_sys();
		auto& texs   = Context::get().textures();
		switch (backend) {
			case ERenderer::vulkan: {
				Resource resource = texs.reserve();

				jobs->add_job(EJobPriority::high, [rel_path, resource, params]() {
					auto& texs    = Context::get().textures();
					auto& plugins = Context::get().plugins();

					auto tex = new vulkan::Texture(resource.id(), rel_path, params);
					auto* r  = static_cast<vulkan::Renderer*>(Context::get().renderer());

					texs.add(resource, tex);
				});

				return create_resource(resource, Context::get().textures());
			}
			default:
				aby_rhi_assert(false, "unimplemented renderer backend");
		}
		return nullptr;
	}

	auto Texture::create_render_target(uint8_t channels, EAntiAliasing aliasing) -> ResourcePtr<Texture, EResource::texture> {
		aby_rhi_assert(Context::get().job_sys(), "context was not initialized");
		auto backend = Context::get().renderer_backend();
		auto* jobs   = Context::get().job_sys();
		auto& texs   = Context::get().textures();
		switch (backend) {
			case ERenderer::vulkan: {
				Resource resource = texs.reserve();

				jobs->add_job(EJobPriority::high, [resource, channels, aliasing]() {
					auto& texs    = Context::get().textures();
					auto& plugins = Context::get().plugins();
					auto* r       = static_cast<vulkan::Renderer*>(Context::get().renderer());

					vk::SampleCountFlagBits samples;
					switch (aliasing) {
						case EAntiAliasing::none:
							samples = vk::SampleCountFlagBits::e1;
							break;
						case EAntiAliasing::msaa2x:
							samples = vk::SampleCountFlagBits::e2;
							break;
						case EAntiAliasing::msaa4x:
							samples = vk::SampleCountFlagBits::e4;
							break;
						case EAntiAliasing::msaa8x:
							samples = vk::SampleCountFlagBits::e8;
							break;
					}

					auto tex = new vulkan::Texture(resource.id(), r->width(), r->height(), channels, samples);
					texs.add(resource, tex);
				});

				return create_resource(resource, Context::get().textures());
			}
			default:
				aby_rhi_assert(false, "unimplemented renderer backend");
				break;
		}
		return nullptr;
	}

	auto TextureParams::set_mip_levels(uint32_t mip_levels) -> TextureParams& {
		this->mip_levels = mip_levels;
		return *this;
	}

	auto TextureParams::set_anisotropy_filtering(float anisotropy_filtering) -> TextureParams& {
		this->anisotropy_filtering = anisotropy_filtering;
		return *this;
	}

	auto TextureParams::set_filtering(EFiltering filtering) -> TextureParams& {
		this->filtering = filtering;
		return *this;
	}

	auto TextureParams::set_repeat_mode(ERepeatMode repeat_mode) -> TextureParams& {
		this->repeat_mode = repeat_mode;
		return *this;
	}

	auto TextureParams::set_texture_usage(ETextureUsage texture_usage) -> TextureParams& {
		this->texture_usage = texture_usage;
		return *this;
	}

	auto TextureParams::set_channels(EChannels channels) -> TextureParams& {
		this->channels = channels;
		return *this;
	}

} // namespace aby::rhi
