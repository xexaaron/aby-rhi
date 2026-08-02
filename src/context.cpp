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
		if (!m_Logger) set_interface<DefaultLogger>();
		if (!m_Allocator) set_interface<DefaultAllocator>();
		if (!m_FileIO) set_interface<DefaultFileIO>();

		m_RendererBackend = renderer_backend;
		m_WinBackend      = window_backend;
		m_Window          = native_window;

		if (m_Renderer = Renderer::create(m_RendererBackend); !m_Renderer) return false;
		if (!m_Renderer->init(m_Window)) return false;

		return true;
	}

	auto Context::deinit() -> void {
		m_Renderer->deinit();
		if (m_Renderer) delete m_Renderer;
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

} // namespace aby::rhi
