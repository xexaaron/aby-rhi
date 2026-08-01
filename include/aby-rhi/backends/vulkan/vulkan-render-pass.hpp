#pragma once
#include "backends/vulkan/vulkan-descriptors.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "render-pass.hpp"

#include <vector>

namespace aby::rhi::vulkan {

	class RenderPass : public rhi::RenderPass {
	public:
		RenderPass(std::unique_ptr<Pipeline> pipeline, const std::vector<std::shared_ptr<rhi::Shader>>& shaders);

		auto bind() -> void override;
		auto run() -> void override;
		auto destroy() -> void override;
		auto set_viewport(vec2<float> size, vec2<float> loc = { 0.f, 0.f }, vec2<float> min_max_depth = { 0.f, 1.f }) -> void override;
		auto set_scissor(vec2<float> offset, vec2<float> size) -> void override;

		auto set_bind_point(vk::PipelineBindPoint point) -> RenderPass&;
		auto set_cmd_buffer(vk::CommandBuffer cmd) -> RenderPass&;
	private:
		vk::PipelineBindPoint m_BindPoint;
		vk::CommandBuffer m_Cmd;
		std::unique_ptr<Pipeline> m_Pipeline;
		std::vector<std::shared_ptr<rhi::Shader>> m_Shaders;
	};

	class RenderPassBuilder : public rhi::RenderPassBuilder {
	public:
		auto build() -> std::shared_ptr<rhi::RenderPass> override;

		auto add_shader(const fs::path& rel_path) -> RenderPassBuilder& override;
		auto add_shader(std::shared_ptr<rhi::Shader> shader) -> RenderPassBuilder& override;
		auto add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder& override;

		auto set_topology(ETopology topology) -> RenderPassBuilder& override;
		auto set_polygon_mode(EPolygonMode mode) -> RenderPassBuilder& override;
		auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& override;

		auto disable_multisampling() -> RenderPassBuilder& override;
		auto disable_blending() -> RenderPassBuilder& override;
		auto disable_depthtest() -> RenderPassBuilder& override;
	private:
		PipelineBuilder m_PipelineBuilder;
		std::vector<std::shared_ptr<rhi::Shader>> m_Shaders;
	};

} // namespace aby::rhi::vulkan
