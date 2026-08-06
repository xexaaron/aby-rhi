#include "context.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "interfaces/default_interfaces.hpp"

#include <assert.h>

namespace aby::rhi {

	auto Context::get() -> Context& {
		static Context context;
		return context;
	}

	auto Context::init(const ContextParams& params) -> bool {
		m_Params = params;
		aby_rhi_profile("context initialization");
		{
			{
				aby_rhi_profile("context interface initialization");
				if (!m_Logger) set_interface<DefaultLogger>();
				if (!m_Allocator) set_interface<DefaultAllocator>();
				if (!m_FileIO) set_interface<DefaultFileIO>();
				if (!m_JobSystem) set_interface<DefaultJobSystem>();
			}

			{
				aby_rhi_profile("context renderer initialization");

				if (m_Renderer = Renderer::create(m_Params.renderer_backend, m_Params.graphics); !m_Renderer) return false;
				if (!m_Renderer->init(m_Params.native_window)) return false;
			}
		}
		return true;
	}

	auto Context::deinit() -> void {
		if (m_JobSystem) delete m_JobSystem;
		{
			aby_rhi_profile("context renderer deinitialization");
			m_Renderer->deinit();
			if (m_Renderer) delete m_Renderer;
		}
		if (m_Logger) delete m_Logger;
		if (m_Allocator) delete m_Allocator;
		if (m_FileIO) delete m_FileIO;
	}

	auto Context::renderer_backend() const -> ERenderer {
		return m_Params.renderer_backend;
	}

	auto Context::window_backend() const -> EWindow {
		return m_Params.window_backend;
	}

	auto Context::renderer() -> Renderer* {
		return m_Renderer;
	}

	auto Context::logger() -> ILogger* {
		return m_Logger;
	}

	auto Context::allocator() -> IAllocator* {
		return m_Allocator;
	}

	auto Context::file_io() -> IFileIO* {
		return m_FileIO;
	}

	auto Context::job_sys() -> IJobSystem* {
		return m_JobSystem;
	}

	auto Context::textures() -> ResourceContainer<Texture, EResource::texture>& {
		return m_Textures;
	}

	auto Context::shaders() -> ResourceContainer<Shader, EResource::shader>& {
		return m_Shaders;
	}

} // namespace aby::rhi
