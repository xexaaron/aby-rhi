#pragma once
#include "plugins/plugin.hpp"

namespace aby::rhi {

	struct ImGuiConfig {
		bool dockspace    = true;
		bool nav_keyboard = true;
		bool nav_gamepad  = true;
		float scale       = 1.f;
	};

	class ImGuiPlugin : public Plugin {
	public:
		ImGuiPlugin(const ImGuiConfig& config);

		auto on_init(EInitTime init_time) -> bool override;
		auto on_begin() -> void override;
		auto on_end() -> void override;
		auto on_deinit(EDeinitTime deinit_time) -> void override;
		auto on_resource_loaded(ResourceID id, EResource type) -> void override;
		auto on_resource_unloaded(ResourceID id, EResource type) -> void override;
		auto on_resource_failed(ResourceID id, EResource type) -> bool override;
	private:
		ImGuiConfig m_Config;
	};

} // namespace aby::rhi
