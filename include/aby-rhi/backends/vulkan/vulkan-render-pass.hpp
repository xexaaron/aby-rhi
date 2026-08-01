#pragma once
#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-descriptors.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "render-pass.hpp"

#include <vector>

namespace aby::rhi::vulkan {

	struct Uniform {
		size_t binding;
		vk::ShaderStageFlags stages;
		vk::DescriptorSet set;
		vulkan::Buffer buffer;
	};

	class RenderPass : public rhi::RenderPass {
	public:
		RenderPass(std::unique_ptr<Pipeline> pipeline,
		           const std::vector<std::shared_ptr<rhi::Shader>>& shaders,
		           const std::unordered_map<std::string, Uniform>& uniforms);

		auto set_uniform(std::string_view name, void* data, size_t bytes) -> void override;

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
		std::unordered_map<std::string, Uniform> m_Uniforms;
	};

	class RenderPassBuilder : public rhi::RenderPassBuilder {
	public:
		RenderPassBuilder();

		auto build() -> std::shared_ptr<rhi::RenderPass> override;
		auto clear() -> void override;

		auto add_shader(const fs::path& rel_path) -> RenderPassBuilder& override;
		auto add_shader(std::shared_ptr<rhi::Shader> shader) -> RenderPassBuilder& override;
		auto add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder& override;

		auto set_topology(ETopology topology) -> RenderPassBuilder& override;
		auto set_polygon_mode(EPolygonMode mode) -> RenderPassBuilder& override;
		auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& override;
		auto set_color_attachment_format(EFormat format) -> RenderPassBuilder& override;
		auto set_depth_format(EFormat format) -> RenderPassBuilder& override;

		auto disable_multisampling() -> RenderPassBuilder& override;
		auto disable_blending() -> RenderPassBuilder& override;
		auto disable_depthtest() -> RenderPassBuilder& override;

		auto use_default_attachment_formats() -> RenderPassBuilder& override;
	private:
		std::vector<std::shared_ptr<rhi::Shader>> m_Shaders;
		vk::PipelineLayout m_PipelineLayout;
		vk::PipelineInputAssemblyStateCreateInfo m_InputAssembly;
		vk::PipelineRasterizationStateCreateInfo m_Rasterizer;
		vk::PipelineColorBlendAttachmentState m_ColorBlendAttachment;
		vk::PipelineMultisampleStateCreateInfo m_Multisampling;
		vk::PipelineDepthStencilStateCreateInfo m_DepthStencil;
		vk::PipelineRenderingCreateInfo m_RenderInfo;
		vk::Format m_ColorAttachmentFormat;
		std::vector<vk::VertexInputBindingDescription> m_VertexInputBindings;
		std::vector<vk::VertexInputAttributeDescription> m_VertexAttributes;
		std::vector<vk::PipelineShaderStageCreateInfo> m_ShaderStages;
		std::vector<vk::DescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<vk::DescriptorSet> m_DescriptorSets;
		std::unordered_map<std::string, vk::DescriptorSetLayoutBinding> m_UniformBindings;
		std::unordered_map<std::string, Uniform> m_Uniforms;
	};

} // namespace aby::rhi::vulkan
