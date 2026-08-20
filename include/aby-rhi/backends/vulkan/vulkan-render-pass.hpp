#pragma once
#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-descriptors.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "backends/vulkan/vulkan-shader.hpp"
#include "render-pass.hpp"
#include "resource.hpp"

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
		           const std::vector<ShaderPtr>& shaders,
		           const std::unordered_map<std::string, Uniform>& uniforms,
		           const std::vector<rhi::Texture*>& color_attachments,
		           const std::vector<rhi::Texture*>& resolve_attachments,
		           rhi::Texture* present_attachment);

		auto set_uniform(std::string_view name, void* data, size_t bytes) -> void override;

		auto bind() -> void override;
		auto begin() -> void override;
		auto end() -> void override;
		auto run() -> void override;
		auto destroy() -> void override;
		auto set_viewport(vec2<float> size, vec2<float> loc = { 0.f, 0.f }, vec2<float> min_max_depth = { 0.f, 1.f }) -> void override;
		auto set_scissor(vec2<float> offset, vec2<float> size) -> void override;
		auto is_present() const -> bool;
		auto present_attachment() -> rhi::Texture*;
		auto color_attachments() -> std::vector<rhi::Texture*>&;
		auto resolve_attachments() -> std::vector<rhi::Texture*>&;

		auto set_bind_point(vk::PipelineBindPoint point) -> RenderPass&;
		auto set_cmd_buffer(vk::CommandBuffer cmd) -> RenderPass&;
	private:
		vk::PipelineBindPoint m_BindPoint;
		vk::CommandBuffer m_Cmd;
		std::unique_ptr<Pipeline> m_Pipeline;
		std::vector<ShaderPtr> m_Shaders;
		std::unordered_map<std::string, Uniform> m_Uniforms;
		std::vector<rhi::Texture*> m_ColorAttachments;
		std::vector<rhi::Texture*> m_ResolveAttachments;
		rhi::Texture* m_PresentAttachment;
	};

	class RenderPassBuilder : public rhi::RenderPassBuilder {
	public:
		RenderPassBuilder();

		auto build() -> std::shared_ptr<rhi::RenderPass> override;
		auto clear() -> void override;

		auto add_shader(const fs::path& rel_path) -> RenderPassBuilder& override;
		auto add_shader(ShaderPtr shader) -> RenderPassBuilder& override;
		auto add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder& override;
		auto add_color_attachment(Resource texture, bool is_present_target = false) -> RenderPassBuilder& override;

		auto set_topology(ETopology topology) -> RenderPassBuilder& override;
		auto set_polygon_mode(EPolygonMode mode, float line_width) -> RenderPassBuilder& override;
		auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& override;
		auto set_depth_format(EFormat format) -> RenderPassBuilder& override;
		auto set_depth(bool enable_test, bool enable_write, ECompareOp compare_op) -> RenderPassBuilder& override;
		auto set_stencil(bool enable, ECompareOp compare_op) -> RenderPassBuilder& override;
		auto set_blend_color(bool enable, Blend blend, size_t attachment = 0) -> RenderPassBuilder& override;
		auto set_blend_alpha(Blend blend, size_t attachment = 0) -> RenderPassBuilder& override;
		auto set_blend_mask(EChannels mask, size_t attachment = 0) -> RenderPassBuilder& override;
		auto set_antialiasing(EAntiAliasing aliasing) -> RenderPassBuilder& override;

		auto disable_blending() -> RenderPassBuilder& override;
		auto disable_depthtest() -> RenderPassBuilder& override;

		auto use_default_attachment_formats() -> RenderPassBuilder& override;
	private:
		Resource m_PresentAttachment;
		size_t m_PresentAttachmentIdx;
		vk::Format m_ColorAttachmentFormat;
		vk::PipelineLayout m_PipelineLayout;
		vk::PipelineInputAssemblyStateCreateInfo m_InputAssembly;
		vk::PipelineRasterizationStateCreateInfo m_Rasterizer;
		vk::PipelineMultisampleStateCreateInfo m_Multisampling;
		vk::PipelineDepthStencilStateCreateInfo m_DepthStencil;
		vk::PipelineRenderingCreateInfo m_RenderInfo;
		vk::SampleCountFlagBits m_SampleCount;
		std::vector<ShaderPtr> m_Shaders;
		std::vector<vk::VertexInputBindingDescription> m_VertexInputBindings;
		std::vector<vk::VertexInputAttributeDescription> m_VertexAttributes;
		std::vector<vk::PipelineShaderStageCreateInfo> m_ShaderStages;
		std::vector<vk::DescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<vk::DescriptorSet> m_DescriptorSets;
		std::vector<vk::Format> m_ColorAttachmentFormats;
		std::vector<Resource> m_ColorAttachments;
		std::vector<vk::PipelineColorBlendAttachmentState> m_ColorBlendAttachments;
		std::unordered_map<std::string, vk::DescriptorSetLayoutBinding> m_UniformBindings;
		std::unordered_map<std::string, Uniform> m_Uniforms;
	};

} // namespace aby::rhi::vulkan
