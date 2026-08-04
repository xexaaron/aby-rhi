#include "context.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "interfaces/default_interfaces.hpp"

#include <assert.h>

namespace aby::rhi {

	auto Context::get() -> Context& {
		static Context context;
		return context;
	}

	auto Context::init(ERenderer renderer_backend, EWindow window_backend, void* native_window) -> bool {
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
				m_RendererBackend = renderer_backend;
				m_WinBackend      = window_backend;
				m_Window          = native_window;
				if (m_Renderer = Renderer::create(m_RendererBackend); !m_Renderer) return false;
				if (!m_Renderer->init(m_Window)) return false;
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
		return m_RendererBackend;
	}

	auto Context::window_backend() const -> EWindow {
		return m_WinBackend;
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
