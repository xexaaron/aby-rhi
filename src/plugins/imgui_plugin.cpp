#include "plugins/imgui_plugin.hpp"

#if ABY_RHI_ENABLE_IMGUI == 1

#	include <imgui.h>

namespace aby::rhi {

	ImGuiPlugin::ImGuiPlugin(const ImGuiConfig& config) :
	    m_Config(config) {
	}

	auto ImGuiPlugin::on_init(EInitTime init_time) -> bool {
		switch (init_time) {
			case EInitTime::pre:
			case EInitTime::main:
				break;
			case EInitTime::post: {
				ImGui::CreateContext();

				auto& io    = ImGui::GetIO();
				auto& style = ImGui::GetStyle();

				if (m_Config.nav_keyboard) {
					io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				}
				if (m_Config.nav_gamepad) {
					io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
				}
				if (m_Config.dockspace) {
					io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				}

				style.ScaleAllSizes(m_Config.scale);
				style.FontScaleDpi = m_Config.scale;
			}
		}
	}

	auto ImGuiPlugin::on_begin() -> void {
	}

	auto ImGuiPlugin::on_end() -> void {
	}

	auto ImGuiPlugin::on_deinit(EDeinitTime deinit_time) -> void {
	}

	auto ImGuiPlugin::on_resource_loaded(ResourceID id, EResource type) -> void {
	}

	auto ImGuiPlugin::on_resource_unloaded(ResourceID id, EResource type) -> void {
	}

	auto ImGuiPlugin::on_resource_failed(ResourceID id, EResource type) -> bool {
	}

} // namespace aby::rhi

#endif
