#include "context.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "interfaces/default_interfaces.hpp"

#include <assert.h>

namespace aby::rhi {

#ifndef NDEBUG
	class DebugPlugin : public Plugin {
	public:
		auto on_init(EInitTime init_time) -> bool override {
			if (init_time == EInitTime::post) {
				aby_rhi_dbg("initialization finished");
			}
			return true;
		}
		auto on_deinit(EDeinitTime deinit_time) -> void override {
			if (deinit_time == EDeinitTime::post) {
				aby_rhi_dbg("deinitialization finished");
			}
		}

		auto on_resource_loaded(ResourceID id, EResource type) -> void override {
			aby_rhi_dbg("loaded resource: [type: {}, id: {}]", type, id);
		}

		auto on_resource_unloaded(ResourceID id, EResource type) -> void override {
			aby_rhi_dbg("unloaded resource: [type: {}, id: {}]", type, id);
		}

		auto on_resource_failed(ResourceID id, EResource type) -> bool override {
			aby_rhi_err("failed to load resource: [type: {}, id: {}]", type, id);
			return true;
		}
	};
#endif

} // namespace aby::rhi

namespace aby::rhi {

	auto Context::get() -> Context& {
		static Context context;
		return context;
	}

	auto Context::init(const ContextParams& params) -> bool {
		aby_rhi_profile("context initialization");

#ifndef NDEBUG
		register_plugin(new DebugPlugin());
#endif

		m_Params = params;
		m_Textures.set_plugins(m_Plugins);
		m_Shaders.set_plugins(m_Plugins);

		{
			m_InitTime = EInitTime::pre;

			for (auto* plugin : m_Plugins) {
				plugin->on_init(EInitTime::pre);
			}

			m_InitTime = EInitTime::main;

			{
				aby_rhi_profile("context interface initialization");
				if (!m_Logger) set_interface<DefaultLogger>();
				if (!m_Allocator) set_interface<DefaultAllocator>();
				if (!m_FileIO) set_interface<DefaultFileIO>();
				if (!m_JobSystem) set_interface<DefaultJobSystem>();
			}

			for (auto* plugin : m_Plugins) {
				plugin->on_init(EInitTime::main);
			}

			{
				aby_rhi_profile("context renderer initialization");

				if (m_Renderer = Renderer::create(m_Params.renderer_backend, m_Params.graphics); !m_Renderer) return false;
				if (!m_Renderer->init(m_Params.native_window)) return false;
			}

			m_InitTime = EInitTime::post;

			for (auto* plugin : m_Plugins) {
				plugin->on_init(EInitTime::post);
			}
		}

		m_InitTime = EInitTime::done;
		return true;
	}

	auto Context::deinit() -> void {
		for (auto* plugin : m_Plugins) {
			plugin->on_deinit(EDeinitTime::pre);
		}
		if (m_JobSystem) delete m_JobSystem;
		{
			aby_rhi_profile("resource deinitialization");
			m_Textures.clear();
			m_Shaders.clear();
		}
		for (auto* plugin : m_Plugins) {
			plugin->on_deinit(EDeinitTime::main);
		}
		{
			aby_rhi_profile("context renderer deinitialization");
			m_Renderer->deinit();
			if (m_Renderer) delete m_Renderer;
		}
		if (m_Allocator) delete m_Allocator;
		if (m_FileIO) delete m_FileIO;
		for (auto* plugin : m_Plugins) {
			plugin->on_deinit(EDeinitTime::post);
		}

		// Delete after post init, just in case users decide to use our log functions.
		if (m_Logger) delete m_Logger;
	}

	auto Context::register_plugin(Plugin* plugin) -> void {
		aby_rhi_assert(m_InitTime == EInitTime::none, "plugin registration is not allowed during/after context initialization");
		aby_rhi_assert(plugin, "registering a null plugin is invalid");
		m_Plugins.push_back(plugin);
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

	auto Context::plugins() -> std::vector<Plugin*>& {
		return m_Plugins;
	}

} // namespace aby::rhi
