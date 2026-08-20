#include "backends/vulkan/vulkan-render-pass.hpp"

#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-shader.hpp"
#include "backends/vulkan/vulkan-texture.hpp"

namespace aby::rhi::vulkan {

	RenderPass::RenderPass(
	    std::unique_ptr<Pipeline> pipeline,
	    const std::vector<ShaderPtr>& shaders,
	    const std::unordered_map<std::string, Uniform>& uniforms,
	    const std::vector<rhi::Texture*>& color_attachments,
	    const std::vector<rhi::Texture*>& resolve_attachments,
	    rhi::Texture* present_attachment) :
	    m_BindPoint(vk::PipelineBindPoint::eGraphics),
	    m_Cmd(VK_NULL_HANDLE),
	    m_Pipeline(std::move(pipeline)),
	    m_Shaders(shaders),
	    m_Uniforms(uniforms),
	    m_ColorAttachments(color_attachments),
	    m_PresentAttachment(present_attachment),
	    m_ResolveAttachments(resolve_attachments) {
		if (!resolve_attachments.empty()) {
			aby_rhi_assert(resolve_attachments.size() == color_attachments.size(), "each color attachment must have a corresponding resolve attachment at the respective index");
		}
	}

	auto RenderPass::set_uniform(std::string_view name, void* data, size_t bytes) -> void {
		auto& uniform = m_Uniforms.at(std::string(name));

		if (!uniform.buffer.allocation()) {
			uniform.buffer = Buffer(
			    bytes,
			    vk::BufferUsageFlagBits::eUniformBuffer,
			    VMA_MEMORY_USAGE_AUTO);

			vk::DescriptorBufferInfo info(
			    uniform.buffer.operator vk::Buffer(),
			    0, /* offset */
			    bytes);

			vk::WriteDescriptorSet write(
			    uniform.set,
			    uniform.binding,
			    0, /* dst array element */
			    1, /* descriptor count */
			    vk::DescriptorType::eUniformBuffer,
			    nullptr,
			    &info);

			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			vkUpdateDescriptorSets(r->device(), 1, vkcast(write), 0, nullptr);
		}

		uniform.buffer.write(data, bytes);
	}

	auto RenderPass::bind() -> void {
		m_Pipeline->bind(m_Cmd, m_BindPoint);
	}

	auto RenderPass::begin() -> void {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		std::vector<vk::RenderingAttachmentInfo> attachments;

		if (m_ResolveAttachments.empty()) {
			for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
				auto* color_attachment = static_cast<vulkan::Texture*>(m_ColorAttachments[i]);

				color_attachment->image().transition(m_Cmd, vk::ImageLayout::eColorAttachmentOptimal);

				attachments.push_back(vk::RenderingAttachmentInfo(
				    color_attachment->view(),
				    color_attachment->image().layout(),
				    vk::ResolveModeFlagBits::eNone,
				    nullptr,
				    vk::ImageLayout::eUndefined,
				    vk::AttachmentLoadOp::eClear,
				    vk::AttachmentStoreOp::eStore,
				    r->clear_color()));
			}
		} else {
			for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
				auto* color_attachment   = static_cast<vulkan::Texture*>(m_ColorAttachments[i]);
				auto* resolve_attachment = static_cast<vulkan::Texture*>(m_ResolveAttachments[i]);

				color_attachment->image().transition(m_Cmd, vk::ImageLayout::eColorAttachmentOptimal);
				resolve_attachment->image().transition(m_Cmd, vk::ImageLayout::eColorAttachmentOptimal);

				attachments.push_back(vk::RenderingAttachmentInfo(
				    color_attachment->view(),
				    color_attachment->image().layout(),
				    vk::ResolveModeFlagBits::eAverage,
				    resolve_attachment->view(),
				    resolve_attachment->image().layout(),
				    vk::AttachmentLoadOp::eClear,
				    vk::AttachmentStoreOp::eStore,
				    r->clear_color()));
			}
		}

		vk::RenderingInfo render_info(
		    vk::RenderingFlags{},
		    vk::Rect2D(
		        vk::Offset2D{},
		        vk::Extent2D{ r->width(), r->height() }),
		    1,                  /* layer count*/
		    0,                  /* view mask */
		    attachments.size(), /* color attachment count */
		    attachments.data());

		vkCmdBeginRendering(m_Cmd, vkcast(render_info));
	}

	auto RenderPass::end() -> void {
		vkCmdEndRendering(m_Cmd);
		for (auto& color_attachment : m_ColorAttachments) {
			auto* attachment = static_cast<vulkan::Texture*>(color_attachment);
			attachment->image().transition(m_Cmd, vk::ImageLayout::eTransferSrcOptimal);
		}

		for (auto& resolve_attachment : m_ResolveAttachments) {
			auto* attachment = static_cast<vulkan::Texture*>(resolve_attachment);
			attachment->image().transition(m_Cmd, vk::ImageLayout::eTransferSrcOptimal);
		}
	}

	auto RenderPass::run() -> void {
		for (auto& cmd : m_Commands) {
			auto* i = static_cast<vulkan::IndexBuffer*>(cmd.ibuff());
			auto* v = static_cast<vulkan::VertexBuffer*>(cmd.vbuff());
			auto s  = cmd.instances();

			VkBuffer vb            = v->gpu();
			VkBuffer ib            = i->gpu();
			VkDeviceSize vb_offset = 0;

			vkCmdBindVertexBuffers(m_Cmd, 0, 1, &vb, &vb_offset);
			vkCmdBindIndexBuffer(m_Cmd, ib, 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(m_Cmd, i->count(), s, 0, 0, 0);
		}
	}

	auto RenderPass::destroy() -> void {
		auto& shaders = Context::get().shaders();
		for (auto& cmd : m_Commands) {
			cmd.vbuff()->destroy();
			cmd.ibuff()->destroy();
		}
		for (auto& [name, uniform] : m_Uniforms) {
			uniform.buffer.destroy();
			std::memset(&uniform, 0, sizeof(Uniform));
		}
		m_Pipeline->destroy();
	}

	auto RenderPass::set_viewport(vec2<float> size, vec2<float> loc, vec2<float> min_max_depth) -> void {
		/* flip the y for vulkan rendering */
		vk::Viewport vp(loc.x, size.y, size.x, -size.y, min_max_depth.x, min_max_depth.y);
		vkCmdSetViewport(m_Cmd, 0, 1, vkcast(vp));
	}

	auto RenderPass::set_scissor(vec2<float> offset, vec2<float> size) -> void {
		vk::Rect2D scissor(
		    vk::Offset2D(offset.x, offset.y),
		    vk::Extent2D(size.x, size.y));
		vkCmdSetScissor(m_Cmd, 0, 1, vkcast(scissor));
	}

	auto RenderPass::set_bind_point(vk::PipelineBindPoint point) -> RenderPass& {
		m_BindPoint = point;
		return *this;
	}

	auto RenderPass::set_cmd_buffer(vk::CommandBuffer cmd) -> RenderPass& {
		m_Cmd = cmd;
		return *this;
	}

	auto RenderPass::is_present() const -> bool {
		return m_PresentAttachment != nullptr;
	}

	auto RenderPass::present_attachment() -> rhi::Texture* {
		return m_PresentAttachment;
	}

	auto RenderPass::color_attachments() -> std::vector<rhi::Texture*>& {
		return m_ColorAttachments;
	}

	auto RenderPass::resolve_attachments() -> std::vector<rhi::Texture*>& {
		return m_ResolveAttachments;
	}

} // namespace aby::rhi::vulkan

namespace aby::rhi::vulkan {

	RenderPassBuilder::RenderPassBuilder() {
		clear();
	}

	auto RenderPassBuilder::build() -> std::shared_ptr<rhi::RenderPass> {
		auto& ctx              = Context::get();
		auto* r                = static_cast<vulkan::Renderer*>(ctx.renderer());
		auto& shader_container = ctx.shaders();

		aby_rhi_assert(!m_ColorAttachments.empty(), "the render pass builder must have atleast one color attachment. either create one or call use_default_attachments_formats()");

		if (!m_VIDB.inputs().empty()) {
			m_VertexInputBindings.push_back(vk::VertexInputBindingDescription(
			    0, /* binding */
			    m_VIDB.stride(),
			    vk::VertexInputRate::eVertex));

			auto& inputs = m_VIDB.inputs();

			for (size_t i = 0; i < inputs.size(); i++) {
				auto& input       = inputs[i];
				vk::Format format = vkconvert(input.format);
				m_VertexAttributes.push_back(vk::VertexInputAttributeDescription(
				    i, /* location */
				    0, /* binding */
				    format,
				    inputs[i].offset));
			}
		}

		for (ShaderPtr shader : m_Shaders) {
			auto* s = static_cast<vulkan::Shader*>(shader.get());
			if (!s) {
				aby_rhi_err("failed to load/retrieve shader: {}", shader.id());
				return nullptr;
			}

			vk::ShaderStageFlagBits stage;

			switch (s->type()) {
				case EShader::none:
					stage = vk::ShaderStageFlagBits::eAll;
					break;
				case EShader::vert:
					stage = vk::ShaderStageFlagBits::eVertex;
					break;
				case EShader::frag:
					stage = vk::ShaderStageFlagBits::eFragment;
					break;
				case EShader::comp:
					stage = vk::ShaderStageFlagBits::eCompute;
					break;
				case EShader::geom:
					stage = vk::ShaderStageFlagBits::eGeometry;
					break;
				default:
					break;
			}

			vk::PipelineShaderStageCreateInfo create_info(
			    vk::PipelineShaderStageCreateFlags(),
			    stage,
			    s->module(),
			    "main");

			m_ShaderStages.push_back(create_info);
		}

		m_DescriptorSets.push_back(r->tex_desc_set());
		m_DescriptorSetLayouts.push_back(r->tex_desc_layout());

		if (!m_UniformBindings.empty()) {
			std::vector<vk::DescriptorSetLayoutBinding> bindings;
			bindings.reserve(m_UniformBindings.size());
			for (auto& [name, binding] : m_UniformBindings) {
				bindings.push_back(binding);
			}

			vk::DescriptorSetLayoutCreateInfo descriptor_layout_ci(
			    vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
			    bindings.size(),
			    bindings.data());

			vk::DescriptorSetLayout layout;

			vkCreateDescriptorSetLayout(
			    r->device(),
			    descriptor_layout_ci,
			    allocator(),
			    vkcast(layout));
			m_DescriptorSetLayouts.push_back(layout);

			auto& desc_alloc = r->desc_alloc();
			m_DescriptorSets.push_back(desc_alloc.alloc(layout));

			r->gc().push([layouts = m_DescriptorSetLayouts]() {
				auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
				/// skip one because the first index will be our texture descriptor array.
				/// we dont own the texture descriptor array and it will be cleaned up by
				/// the renderer.
				for (size_t i = 1; i < layouts.size(); i++) {
					vkDestroyDescriptorSetLayout(
					    r->device(),
					    layouts[i],
					    allocator());
				}
			});

			for (auto& [name, uniform] : m_Uniforms) {
				uniform.set = m_DescriptorSets.back();
			}
		}

		m_Multisampling.setRasterizationSamples(m_SampleCount)
		    .setSampleShadingEnable(vk::False)
		    .setMinSampleShading(1.0f)
		    .setPSampleMask(VK_NULL_HANDLE)
		    .setAlphaToCoverageEnable(vk::False)
		    .setAlphaToOneEnable(vk::False);

		EAntiAliasing aliasing = EAntiAliasing::none;
		switch (m_SampleCount) {
			case vk::SampleCountFlagBits::e1:
				aliasing = EAntiAliasing::none;
				break;
			case vk::SampleCountFlagBits::e2:
				aliasing = EAntiAliasing::msaa2x;
				break;
			case vk::SampleCountFlagBits::e4:
				aliasing = EAntiAliasing::msaa4x;
				break;
			case vk::SampleCountFlagBits::e8:
				aliasing = EAntiAliasing::msaa8x;
				break;
		}

		// We want to do this as late as possible because textures may still be loading
		// and this forces us to wait for them
		std::vector<rhi::Texture*> textures;
		std::vector<rhi::Texture*> resolve_attachments;
		if (!m_ColorAttachments.empty()) {
			for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
				auto resource = m_ColorAttachments[i];
				auto& texs    = Context::get().textures();
				auto* tex     = texs[resource];

				aby_rhi_assert(static_cast<Texture*>(tex)->image().samples() == m_SampleCount,
				               "color attachment ({}) sample count does not match render pass anti aliasing sample count: {}", i, aliasing);

				textures.push_back(tex);
				m_ColorAttachmentFormats.push_back(static_cast<vulkan::Texture*>(tex)->format());

				if (m_SampleCount != vk::SampleCountFlagBits::e1) {
					// create a resolve texture with 1 sample count bit.
					// TODO: change this to load the textures in two phases. storing the resource handles first
					auto resolve_resource = Texture::create_render_target(tex->channels(), aby::rhi::EAntiAliasing::none);
					resolve_attachments.push_back(resolve_resource.get());
				}
			}

			m_RenderInfo.setColorAttachmentFormats(m_ColorAttachmentFormats);
		} else {
			use_default_attachment_formats();
		}

		vk::PipelineVertexInputStateCreateInfo vertex_input_info(
		    vk::PipelineVertexInputStateCreateFlags(),
		    m_VertexInputBindings.size(),
		    m_VertexInputBindings.data(),
		    m_VertexAttributes.size(),
		    m_VertexAttributes.data());

		vk::PipelineTessellationStateCreateInfo tesselation_input_info;
		vk::PipelineViewportStateCreateInfo viewport_state(
		    vk::PipelineViewportStateCreateFlags{},
		    1,       /* viewport count */
		    nullptr, /* viewports */
		    1,       /* scissor count */
		    nullptr  /* scissors */
		);

		auto dynamic_states = std::to_array<vk::DynamicState>({ vk::DynamicState::eViewport,
		                                                        vk::DynamicState::eScissor });

		vk::PipelineDynamicStateCreateInfo dynamic_state_info(
		    vk::PipelineDynamicStateCreateFlags{},
		    dynamic_states.size(),
		    dynamic_states.data());

		vk::PipelineColorBlendStateCreateInfo color_blend_state(
		    vk::PipelineColorBlendStateCreateFlags(),
		    vk::False, /* logic op enable */
		    vk::LogicOp::eCopy,
		    m_ColorBlendAttachments.size(),
		    m_ColorBlendAttachments.data());

		vk::PipelineLayoutCreateInfo layout_create_info(
		    vk::PipelineLayoutCreateFlags(),
		    m_DescriptorSetLayouts.size(),
		    m_DescriptorSetLayouts.data(),
		    0,      /* push constant ranges count */
		    nullptr /*  push constant ranges      */
		);

		vkassert(vkCreatePipelineLayout(
		             r->device(),
		             vkcast(layout_create_info),
		             allocator(),
		             vkcast(m_PipelineLayout)),
		         "failed to create pipeline layout");

		vk::GraphicsPipelineCreateInfo create_info(
		    vk::PipelineCreateFlags{},
		    static_cast<uint32_t>(m_ShaderStages.size()),
		    m_ShaderStages.data(),
		    &vertex_input_info,
		    &m_InputAssembly,
		    &tesselation_input_info,
		    &viewport_state,
		    &m_Rasterizer,
		    &m_Multisampling,
		    &m_DepthStencil,
		    &color_blend_state,
		    &dynamic_state_info,
		    m_PipelineLayout,
		    vk::RenderPass{},
		    0, /* subpass */
		    vk::Pipeline{},
		    0, /* base pipeline index */
		    static_cast<void*>(&m_RenderInfo));

		vk::Pipeline pipeline;

		vkassert(vkCreateGraphicsPipelines(
		             r->device(),
		             VK_NULL_HANDLE, /* pipeline cache */
		             1,              /* pipeline count */
		             vkcast(create_info),
		             allocator(),
		             vkcast(pipeline)),
		         "failed to create graphics pipeline");

		rhi::Texture* present_attachment = nullptr;
		if (m_PresentAttachment) {
			auto& texs         = Context::get().textures();
			present_attachment = texs[m_PresentAttachment];
		}

		rhi::Texture* present = nullptr;
		if (m_PresentAttachmentIdx != SIZE_MAX) {
			if (m_SampleCount != vk::SampleCountFlagBits::e1) {
				present = resolve_attachments[m_PresentAttachmentIdx];
			} else {
				present = present_attachment;
			}
		}

		return std::make_shared<RenderPass>(
		    std::make_unique<Pipeline>(pipeline, m_PipelineLayout, m_DescriptorSets),
		    m_Shaders,
		    m_Uniforms,
		    textures,
		    resolve_attachments,
		    present);
	}

	auto RenderPassBuilder::clear() -> void {
		m_PresentAttachment     = Resource();
		m_ColorAttachmentFormat = vk::Format::eUndefined;
		m_PipelineLayout        = vk::PipelineLayout();
		m_InputAssembly         = vk::PipelineInputAssemblyStateCreateInfo();
		m_Rasterizer            = vk::PipelineRasterizationStateCreateInfo();
		m_Multisampling         = vk::PipelineMultisampleStateCreateInfo();
		m_DepthStencil          = vk::PipelineDepthStencilStateCreateInfo();
		m_RenderInfo            = vk::PipelineRenderingCreateInfo();
		m_SampleCount           = vk::SampleCountFlagBits::e1;
		m_PresentAttachmentIdx  = SIZE_MAX;
		m_Shaders.clear();
		m_VertexInputBindings.clear();
		m_VertexAttributes.clear();
		m_ShaderStages.clear();
		m_DescriptorSetLayouts.clear();
		m_DescriptorSets.clear();
		m_ColorAttachmentFormats.clear();
		m_ColorAttachments.clear();
		m_UniformBindings.clear();
		m_Uniforms.clear();
		m_ColorBlendAttachments.clear();
	}

	auto RenderPassBuilder::add_shader(const fs::path& rel_path) -> RenderPassBuilder& {
		auto shader = Shader::create(rel_path);
		return add_shader(shader);
	}

	auto RenderPassBuilder::add_shader(ShaderPtr shader) -> RenderPassBuilder& {
		aby_rhi_assert(shader.type() == EResource::shader, "attempted to add a shader resource that is not of type EResource::shader");
		m_Shaders.push_back(shader);
		return *this;
	}

	auto RenderPassBuilder::add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder& {
		vk::ShaderStageFlags stage_flags = vkconvert(stage);

		m_UniformBindings[std::string(name)] = vk::DescriptorSetLayoutBinding(
		    binding,
		    vk::DescriptorType::eUniformBuffer,
		    1, /* descriptor count */
		    stage_flags);

		m_Uniforms[std::string(name)] = Uniform{
			.binding = binding,
			.stages  = stage_flags,
			.set     = VK_NULL_HANDLE,
		};

		return *this;
	}

	auto RenderPassBuilder::add_color_attachment(Resource texture, bool is_present_target) -> RenderPassBuilder& {
		m_ColorAttachments.push_back(texture);
		if (is_present_target) {
			aby_rhi_assert(!m_PresentAttachment, "render pass cannot have multiple present attachments. previously set attachment: {}", m_PresentAttachment.id());
			m_PresentAttachment    = texture;
			m_PresentAttachmentIdx = m_ColorAttachments.size() - 1;
		}
		return *this;
	}

	auto RenderPassBuilder::set_topology(ETopology topology) -> RenderPassBuilder& {
		vk::PrimitiveTopology t = vkconvert(topology);

		m_InputAssembly.setTopology(t);
		m_InputAssembly.setPrimitiveRestartEnable(vk::False);
		return *this;
	}

	auto RenderPassBuilder::set_polygon_mode(EPolygonMode mode, float line_width) -> RenderPassBuilder& {
		vk::PolygonMode m = vkconvert(mode);

		m_Rasterizer.setPolygonMode(m);
		m_Rasterizer.setLineWidth(line_width);
		return *this;
	}

	auto RenderPassBuilder::set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& {
		vk::CullModeFlags m = vkconvert(mode);
		vk::FrontFace f     = vkconvert(front_face);

		m_Rasterizer.setCullMode(m);
		m_Rasterizer.setFrontFace(f);
		return *this;
	}

	auto RenderPassBuilder::set_depth_format(EFormat format) -> RenderPassBuilder& {
		m_RenderInfo.setDepthAttachmentFormat(vkconvert(format));
		return *this;
	}

	auto RenderPassBuilder::set_depth(bool enable_test, bool enable_write, ECompareOp compare_op) -> RenderPassBuilder& {
		vk::CompareOp op = vkconvert(compare_op);

		m_DepthStencil.setDepthTestEnable(enable_test);
		m_DepthStencil.setDepthWriteEnable(enable_write);
		m_DepthStencil.setDepthCompareOp(op);
		m_DepthStencil.setMinDepthBounds(0.f);
		m_DepthStencil.setMaxDepthBounds(1.f);
		return *this;
	}

	auto RenderPassBuilder::set_stencil(bool enable, ECompareOp compare_op) -> RenderPassBuilder& {
		vk::CompareOp op = vkconvert(compare_op);

		vk::StencilOpState op_state(
		    vk::StencilOp::eKeep, /* fail op*/
		    vk::StencilOp::eKeep, /* pass op */
		    vk::StencilOp::eKeep, /* depth fail op */
		    op,
		    0xFF,
		    0xFF,
		    0);

		m_DepthStencil.setStencilTestEnable(enable);
		m_DepthStencil.setFront(op_state);
		m_DepthStencil.setBack(op_state);
		return *this;
	}

	auto RenderPassBuilder::set_blend_color(bool enable, Blend blend, size_t attachment) -> RenderPassBuilder& {
		vk::BlendOp op                   = vkconvert(blend.op);
		vk::BlendFactor src_blend_factor = vkconvert(blend.src);
		vk::BlendFactor dst_blend_factor = vkconvert(blend.dst);

		if ((attachment + 1) > m_ColorBlendAttachments.size()) {
			m_ColorBlendAttachments.resize(attachment + 1);
		}

		m_ColorBlendAttachments[attachment].setBlendEnable(enable);
		m_ColorBlendAttachments[attachment].setColorBlendOp(op);
		m_ColorBlendAttachments[attachment].setSrcColorBlendFactor(src_blend_factor);
		m_ColorBlendAttachments[attachment].setDstColorBlendFactor(dst_blend_factor);
		return *this;
	}

	auto RenderPassBuilder::set_blend_alpha(Blend blend, size_t attachment) -> RenderPassBuilder& {
		vk::BlendOp op                   = vkconvert(blend.op);
		vk::BlendFactor src_blend_factor = vkconvert(blend.src);
		vk::BlendFactor dst_blend_factor = vkconvert(blend.dst);

		if ((attachment + 1) > m_ColorBlendAttachments.size()) {
			m_ColorBlendAttachments.resize(attachment + 1);
		}

		m_ColorBlendAttachments[attachment].setAlphaBlendOp(op);
		m_ColorBlendAttachments[attachment].setSrcAlphaBlendFactor(src_blend_factor);
		m_ColorBlendAttachments[attachment].setDstAlphaBlendFactor(dst_blend_factor);
		return *this;
	}

	auto RenderPassBuilder::set_blend_mask(EChannels mask, size_t attachment) -> RenderPassBuilder& {
		vk::ColorComponentFlags flags;
		if ((static_cast<uint8_t>(mask) & static_cast<uint8_t>(EChannels::r)) != 0) flags |= vk::ColorComponentFlagBits::eR;
		if ((static_cast<uint8_t>(mask) & static_cast<uint8_t>(EChannels::g)) != 0) flags |= vk::ColorComponentFlagBits::eG;
		if ((static_cast<uint8_t>(mask) & static_cast<uint8_t>(EChannels::b)) != 0) flags |= vk::ColorComponentFlagBits::eB;
		if ((static_cast<uint8_t>(mask) & static_cast<uint8_t>(EChannels::a)) != 0) flags |= vk::ColorComponentFlagBits::eA;

		if ((attachment + 1) > m_ColorBlendAttachments.size()) {
			m_ColorBlendAttachments.resize(attachment + 1);
		}

		m_ColorBlendAttachments[attachment].setColorWriteMask(flags);
		return *this;
	}

	auto RenderPassBuilder::set_antialiasing(EAntiAliasing aliasing) -> RenderPassBuilder& {
		switch (aliasing) {
			case EAntiAliasing::none:
				m_SampleCount = vk::SampleCountFlagBits::e1;
				break;
			case EAntiAliasing::msaa2x:
				m_SampleCount = vk::SampleCountFlagBits::e2;
				break;
			case EAntiAliasing::msaa4x:
				m_SampleCount = vk::SampleCountFlagBits::e4;
				break;
			case EAntiAliasing::msaa8x:
				m_SampleCount = vk::SampleCountFlagBits::e8;
				break;
			default:
				m_SampleCount = vk::SampleCountFlagBits::e1;
				break;
		}
		return *this;
	}

	auto RenderPassBuilder::disable_blending() -> RenderPassBuilder& {
		m_ColorBlendAttachments.resize(m_ColorAttachments.size());
		for (auto& attachment : m_ColorBlendAttachments) {
			attachment.setColorWriteMask(vk::ColorComponentFlagBits::eR |
			                             vk::ColorComponentFlagBits::eG |
			                             vk::ColorComponentFlagBits::eB |
			                             vk::ColorComponentFlagBits::eA);
			attachment.setBlendEnable(vk::False);
		}
		return *this;
	}

	auto RenderPassBuilder::disable_depthtest() -> RenderPassBuilder& {
		m_DepthStencil.setDepthTestEnable(vk::False);
		m_DepthStencil.setDepthWriteEnable(vk::False);
		m_DepthStencil.setDepthCompareOp(vk::CompareOp::eNever);
		m_DepthStencil.setDepthBoundsTestEnable(vk::False);
		m_DepthStencil.setStencilTestEnable(vk::False);
		m_DepthStencil.setFront(vk::StencilOpState());
		m_DepthStencil.setBack(vk::StencilOpState());
		m_DepthStencil.setMinDepthBounds(0.f);
		m_DepthStencil.setMaxDepthBounds(1.f);
		return *this;
	}

	auto RenderPassBuilder::use_default_attachment_formats() -> RenderPassBuilder& {
		auto* r                 = static_cast<vulkan::Renderer*>(Context::get().renderer());
		m_ColorAttachmentFormat = r->color_format();
		m_RenderInfo.setColorAttachmentCount(1);
		m_RenderInfo.setPColorAttachmentFormats(&m_ColorAttachmentFormat);
		set_depth_format(EFormat::none);

		EAntiAliasing aliasing = EAntiAliasing::none;
		switch (m_SampleCount) {
			case vk::SampleCountFlagBits::e1:
				aliasing = EAntiAliasing::none;
				break;
			case vk::SampleCountFlagBits::e2:
				aliasing = EAntiAliasing::msaa2x;
				break;
			case vk::SampleCountFlagBits::e4:
				aliasing = EAntiAliasing::msaa4x;
				break;
			case vk::SampleCountFlagBits::e8:
				aliasing = EAntiAliasing::msaa8x;
				break;
		}

		add_color_attachment(Texture::create_render_target(4, aliasing), true);

		return *this;
	}

} // namespace aby::rhi::vulkan
