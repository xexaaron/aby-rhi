#include "backends/vulkan/vulkan-render-pass.hpp"

#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-shader.hpp"

namespace aby::rhi::vulkan {

	RenderPass::RenderPass(
	    std::unique_ptr<Pipeline> pipeline,
	    const std::vector<Resource>& shaders,
	    const std::unordered_map<std::string, Uniform>& uniforms) :
	    m_BindPoint(vk::PipelineBindPoint::eGraphics),
	    m_Cmd(VK_NULL_HANDLE),
	    m_Pipeline(std::move(pipeline)),
	    m_Shaders(shaders),
	    m_Uniforms(uniforms) {
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
		for (Resource shader : m_Shaders) {
			shaders.remove(shader);
		}
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

} // namespace aby::rhi::vulkan

namespace aby::rhi::vulkan {

	RenderPassBuilder::RenderPassBuilder() {
		clear();
	}

	auto RenderPassBuilder::build() -> std::shared_ptr<rhi::RenderPass> {
		auto& ctx              = Context::get();
		auto* r                = static_cast<vulkan::Renderer*>(ctx.renderer());
		auto& shader_container = ctx.shaders();

		if (!m_VIDB.inputs().empty()) {
			m_VertexInputBindings.push_back(vk::VertexInputBindingDescription(
			    0, /* binding */
			    m_VIDB.stride(),
			    vk::VertexInputRate::eVertex));

			auto& inputs = m_VIDB.inputs();

			for (size_t i = 0; i < inputs.size(); i++) {
				auto& input       = inputs[i];
				vk::Format format = eformat_to_vkformat(input.format);
				m_VertexAttributes.push_back(vk::VertexInputAttributeDescription(
				    i, /* location */
				    0, /* binding */
				    format,
				    inputs[i].offset));
			}
		}

		if (!m_ColorAttachments.empty()) {
			m_RenderInfo.setColorAttachmentFormats(m_ColorAttachments);
		} else {
			use_default_attachment_formats();
		}

		for (Resource shader : m_Shaders) {
			if (!shader_container.wait_for(shader)) {
				aby_rhi_err("failed to wait for shader: {}", shader.id());
				return nullptr;
			}

			auto* s = static_cast<vulkan::Shader*>(shader_container[shader]);
			if (!s) {
				aby_rhi_err("failed to retrieve shader: {}", shader.id());
				return nullptr;
			}

			vk::ShaderStageFlagBits stage;

			switch (s->type()) {
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
		    1, /* attachment count */
		    &m_ColorBlendAttachment);

		vk::PipelineLayoutCreateInfo layout_create_info(
		    vk::PipelineLayoutCreateFlags(),
		    m_DescriptorSetLayouts.size(),
		    m_DescriptorSetLayouts.data(),
		    0,      /* push constant ranges count */
		    nullptr /*  push constant ranges      */
		);

		m_Multisampling.setRasterizationSamples(r->render_target_sample_count())
		    .setSampleShadingEnable(vk::False)
		    .setMinSampleShading(1.0f)
		    .setPSampleMask(VK_NULL_HANDLE)
		    .setAlphaToCoverageEnable(vk::False)
		    .setAlphaToOneEnable(vk::False);

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

		return std::make_shared<RenderPass>(
		    std::make_unique<Pipeline>(pipeline, m_PipelineLayout, m_DescriptorSets),
		    m_Shaders,
		    m_Uniforms);
	}

	auto RenderPassBuilder::clear() -> void {
		m_PipelineLayout        = vk::PipelineLayout();
		m_InputAssembly         = vk::PipelineInputAssemblyStateCreateInfo();
		m_Rasterizer            = vk::PipelineRasterizationStateCreateInfo();
		m_ColorBlendAttachment  = vk::PipelineColorBlendAttachmentState();
		m_Multisampling         = vk::PipelineMultisampleStateCreateInfo();
		m_DepthStencil          = vk::PipelineDepthStencilStateCreateInfo();
		m_RenderInfo            = vk::PipelineRenderingCreateInfo();
		m_ColorAttachmentFormat = vk::Format::eUndefined;
		m_ShaderStages.clear();
	}

	auto RenderPassBuilder::add_shader(const fs::path& rel_path) -> RenderPassBuilder& {
		auto shader = Shader::create(rel_path);
		return add_shader(shader);
	}

	auto RenderPassBuilder::add_shader(Resource shader) -> RenderPassBuilder& {
		aby_rhi_assert(shader.type() == EResource::shader, "attempted to add a shader resource that is not of type EResource::shader");
		m_Shaders.push_back(shader);
		return *this;
	}

	auto RenderPassBuilder::add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder& {
		vk::ShaderStageFlags stage_flags;

		switch (stage) {
			case EShader::none:
				stage_flags |= vk::ShaderStageFlagBits::eAll;
				break;
			case EShader::vert:
				stage_flags |= vk::ShaderStageFlagBits::eVertex;
				break;
			case EShader::frag:
				stage_flags |= vk::ShaderStageFlagBits::eFragment;
				break;
			case EShader::comp:
				stage_flags |= vk::ShaderStageFlagBits::eCompute;
				break;
			case EShader::geom:
				stage_flags |= vk::ShaderStageFlagBits::eGeometry;
				break;
		}

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

	auto RenderPassBuilder::add_color_attachment(Texture* texture) -> RenderPassBuilder& {
		switch (texture->channels()) {
			case 1:
				m_ColorAttachments.push_back(vk::Format::eR16Sfloat);
				break;
			case 2:
				m_ColorAttachments.push_back(vk::Format::eR16G16Sfloat);
				break;
			case 3:
				m_ColorAttachments.push_back(vk::Format::eR16G16B16Sfloat);
				break;
			case 4:
				m_ColorAttachments.push_back(vk::Format::eR16G16B16A16Sfloat);
				break;
		}
		return *this;
	}

	auto RenderPassBuilder::set_topology(ETopology topology) -> RenderPassBuilder& {
		vk::PrimitiveTopology t;
		switch (topology) {
			case ETopology::point_list:
				t = vk::PrimitiveTopology::ePointList;
				break;
			case ETopology::line_list:
				t = vk::PrimitiveTopology::eLineList;
				break;
			case ETopology::line_strip:
				t = vk::PrimitiveTopology::eLineStrip;
				break;
			case ETopology::triangle_list:
				t = vk::PrimitiveTopology::eTriangleList;
				break;
			case ETopology::triangle_strip:
				t = vk::PrimitiveTopology::eTriangleStrip;
				break;
			case ETopology::triangle_fan:
				t = vk::PrimitiveTopology::eTriangleFan;
				break;
		}
		m_InputAssembly.setTopology(t);
		m_InputAssembly.setPrimitiveRestartEnable(vk::False);
		return *this;
	}

	auto RenderPassBuilder::set_polygon_mode(EPolygonMode mode, float line_width) -> RenderPassBuilder& {
		vk::PolygonMode m;
		switch (mode) {
			case EPolygonMode::fill:
				m = vk::PolygonMode::eFill;
				break;
			case EPolygonMode::line:
				m = vk::PolygonMode::eLine;
				break;
			case EPolygonMode::point:
				m = vk::PolygonMode::ePoint;
				break;
		}
		m_Rasterizer.setPolygonMode(m);
		m_Rasterizer.setLineWidth(line_width);
		return *this;
	}

	auto RenderPassBuilder::set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& {
		vk::CullModeFlags m;
		vk::FrontFace f;

		switch (mode) {
			case ECullMode::none:
				m = vk::CullModeFlagBits::eNone;
				break;
			case ECullMode::front:
				m = vk::CullModeFlagBits::eFront;
				break;
			case ECullMode::back:
				m = vk::CullModeFlagBits::eBack;
				break;
			case ECullMode::front_and_back:
				m = vk::CullModeFlagBits::eFrontAndBack;
				break;
		}

		switch (front_face) {
			case EFrontFace::clockwise:
				f = vk::FrontFace::eClockwise;
				break;
			case EFrontFace::counter_clockwise:
				f = vk::FrontFace::eCounterClockwise;
				break;
		}

		m_Rasterizer.setCullMode(m);
		m_Rasterizer.setFrontFace(f);
		return *this;
	}

	auto RenderPassBuilder::set_depth_format(EFormat format) -> RenderPassBuilder& {
		m_RenderInfo.setDepthAttachmentFormat(eformat_to_vkformat(format));
		return *this;
	}

	auto RenderPassBuilder::set_depth(bool enable_test, bool enable_write, ECompareOp compare_op) -> RenderPassBuilder& {
		vk::CompareOp op;
		switch (compare_op) {
			case ECompareOp::never:
				op = vk::CompareOp::eNever;
				break;
			case ECompareOp::less:
				op = vk::CompareOp::eLess;
				break;
			case ECompareOp::eq:
				op = vk::CompareOp::eEqual;
				break;
			case ECompareOp::less_eq:
				op = vk::CompareOp::eLessOrEqual;
				break;
			case ECompareOp::greater:
				op = vk::CompareOp::eGreater;
				break;
			case ECompareOp::neq:
				op = vk::CompareOp::eNotEqual;
				break;
			case ECompareOp::greater_eq:
				op = vk::CompareOp::eGreaterOrEqual;
				break;
			case ECompareOp::always:
				op = vk::CompareOp::eAlways;
				break;
		}
		m_DepthStencil.setDepthTestEnable(enable_test);
		m_DepthStencil.setDepthWriteEnable(enable_write);
		m_DepthStencil.setDepthCompareOp(op);
		m_DepthStencil.setMinDepthBounds(0.f);
		m_DepthStencil.setMaxDepthBounds(1.f);
		return *this;
	}

	auto RenderPassBuilder::set_stencil(bool enable, ECompareOp compare_op) -> RenderPassBuilder& {
		vk::CompareOp op;
		switch (compare_op) {
			case ECompareOp::never:
				op = vk::CompareOp::eNever;
				break;
			case ECompareOp::less:
				op = vk::CompareOp::eLess;
				break;
			case ECompareOp::eq:
				op = vk::CompareOp::eEqual;
				break;
			case ECompareOp::less_eq:
				op = vk::CompareOp::eLessOrEqual;
				break;
			case ECompareOp::greater:
				op = vk::CompareOp::eGreater;
				break;
			case ECompareOp::neq:
				op = vk::CompareOp::eNotEqual;
				break;
			case ECompareOp::greater_eq:
				op = vk::CompareOp::eGreaterOrEqual;
				break;
			case ECompareOp::always:
				op = vk::CompareOp::eAlways;
				break;
		}

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

	auto RenderPassBuilder::set_blend_color(bool enable, Blend blend) -> RenderPassBuilder& {
		vk::BlendOp op;
		vk::BlendFactor src_blend_factor;
		vk::BlendFactor dst_blend_factor;

		switch (blend.op) {
			case EBlendOp::add:
				op = vk::BlendOp::eAdd;
				break;
			case EBlendOp::sub:
				op = vk::BlendOp::eSubtract;
				break;
			case EBlendOp::reverse_sub:
				op = vk::BlendOp::eReverseSubtract;
				break;
			case EBlendOp::min:
				op = vk::BlendOp::eMin;
				break;
			case EBlendOp::max:
				op = vk::BlendOp::eMax;
				break;
		}

		switch (blend.src) {
			case EBlendFactor::zero:
				src_blend_factor = vk::BlendFactor::eZero;
				break;
			case EBlendFactor::one:
				src_blend_factor = vk::BlendFactor::eOne;
				break;
			case EBlendFactor::src_color:
				src_blend_factor = vk::BlendFactor::eSrcColor;
				break;
			case EBlendFactor::one_minus_src_color:
				src_blend_factor = vk::BlendFactor::eOneMinusSrcColor;
				break;
			case EBlendFactor::dst_color:
				src_blend_factor = vk::BlendFactor::eDstColor;
				break;
			case EBlendFactor::one_minus_dst_color:
				src_blend_factor = vk::BlendFactor::eOneMinusDstColor;
				break;
			case EBlendFactor::src_alpha:
				src_blend_factor = vk::BlendFactor::eSrcAlpha;
				break;
			case EBlendFactor::one_minus_src_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusSrcAlpha;
				break;
			case EBlendFactor::dst_alpha:
				src_blend_factor = vk::BlendFactor::eDstAlpha;
				break;
			case EBlendFactor::one_minus_dst_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusDstAlpha;
				break;
			case EBlendFactor::constant_color:
				src_blend_factor = vk::BlendFactor::eConstantColor;
				break;
			case EBlendFactor::one_minus_constant_color:
				src_blend_factor = vk::BlendFactor::eOneMinusConstantColor;
				break;
			case EBlendFactor::constant_alpha:
				src_blend_factor = vk::BlendFactor::eConstantAlpha;
				break;
			case EBlendFactor::one_minus_constant_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusConstantAlpha;
				break;
			case EBlendFactor::src_alpha_saturate:
				src_blend_factor = vk::BlendFactor::eSrcAlphaSaturate;
				break;
			case EBlendFactor::src_one_color:
				src_blend_factor = vk::BlendFactor::eSrc1Color;
				break;
			case EBlendFactor::one_minus_src_one_color:
				src_blend_factor = vk::BlendFactor::eOneMinusSrc1Color;
				break;
			case EBlendFactor::src_one_alpha:
				src_blend_factor = vk::BlendFactor::eSrc1Alpha;
				break;
			case EBlendFactor::one_minus_src_one_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusSrc1Alpha;
				break;
		}

		switch (blend.dst) {
			case EBlendFactor::zero:
				dst_blend_factor = vk::BlendFactor::eZero;
				break;
			case EBlendFactor::one:
				dst_blend_factor = vk::BlendFactor::eOne;
				break;
			case EBlendFactor::src_color:
				dst_blend_factor = vk::BlendFactor::eSrcColor;
				break;
			case EBlendFactor::one_minus_src_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrcColor;
				break;
			case EBlendFactor::dst_color:
				dst_blend_factor = vk::BlendFactor::eDstColor;
				break;
			case EBlendFactor::one_minus_dst_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusDstColor;
				break;
			case EBlendFactor::src_alpha:
				dst_blend_factor = vk::BlendFactor::eSrcAlpha;
				break;
			case EBlendFactor::one_minus_src_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrcAlpha;
				break;
			case EBlendFactor::dst_alpha:
				dst_blend_factor = vk::BlendFactor::eDstAlpha;
				break;
			case EBlendFactor::one_minus_dst_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusDstAlpha;
				break;
			case EBlendFactor::constant_color:
				dst_blend_factor = vk::BlendFactor::eConstantColor;
				break;
			case EBlendFactor::one_minus_constant_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusConstantColor;
				break;
			case EBlendFactor::constant_alpha:
				dst_blend_factor = vk::BlendFactor::eConstantAlpha;
				break;
			case EBlendFactor::one_minus_constant_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusConstantAlpha;
				break;
			case EBlendFactor::src_alpha_saturate:
				dst_blend_factor = vk::BlendFactor::eSrcAlphaSaturate;
				break;
			case EBlendFactor::src_one_color:
				dst_blend_factor = vk::BlendFactor::eSrc1Color;
				break;
			case EBlendFactor::one_minus_src_one_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrc1Color;
				break;
			case EBlendFactor::src_one_alpha:
				dst_blend_factor = vk::BlendFactor::eSrc1Alpha;
				break;
			case EBlendFactor::one_minus_src_one_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrc1Alpha;
				break;
		}

		m_ColorBlendAttachment.setBlendEnable(enable);
		m_ColorBlendAttachment.setColorBlendOp(op);
		m_ColorBlendAttachment.setSrcColorBlendFactor(src_blend_factor);
		m_ColorBlendAttachment.setDstColorBlendFactor(dst_blend_factor);
		return *this;
	}

	auto RenderPassBuilder::set_blend_alpha(Blend blend) -> RenderPassBuilder& {
		vk::BlendOp op;
		vk::BlendFactor src_blend_factor;
		vk::BlendFactor dst_blend_factor;

		switch (blend.op) {
			case EBlendOp::add:
				op = vk::BlendOp::eAdd;
				break;
			case EBlendOp::sub:
				op = vk::BlendOp::eSubtract;
				break;
			case EBlendOp::reverse_sub:
				op = vk::BlendOp::eReverseSubtract;
				break;
			case EBlendOp::min:
				op = vk::BlendOp::eMin;
				break;
			case EBlendOp::max:
				op = vk::BlendOp::eMax;
				break;
		}

		switch (blend.src) {
			case EBlendFactor::zero:
				src_blend_factor = vk::BlendFactor::eZero;
				break;
			case EBlendFactor::one:
				src_blend_factor = vk::BlendFactor::eOne;
				break;
			case EBlendFactor::src_color:
				src_blend_factor = vk::BlendFactor::eSrcColor;
				break;
			case EBlendFactor::one_minus_src_color:
				src_blend_factor = vk::BlendFactor::eOneMinusSrcColor;
				break;
			case EBlendFactor::dst_color:
				src_blend_factor = vk::BlendFactor::eDstColor;
				break;
			case EBlendFactor::one_minus_dst_color:
				src_blend_factor = vk::BlendFactor::eOneMinusDstColor;
				break;
			case EBlendFactor::src_alpha:
				src_blend_factor = vk::BlendFactor::eSrcAlpha;
				break;
			case EBlendFactor::one_minus_src_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusSrcAlpha;
				break;
			case EBlendFactor::dst_alpha:
				src_blend_factor = vk::BlendFactor::eDstAlpha;
				break;
			case EBlendFactor::one_minus_dst_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusDstAlpha;
				break;
			case EBlendFactor::constant_color:
				src_blend_factor = vk::BlendFactor::eConstantColor;
				break;
			case EBlendFactor::one_minus_constant_color:
				src_blend_factor = vk::BlendFactor::eOneMinusConstantColor;
				break;
			case EBlendFactor::constant_alpha:
				src_blend_factor = vk::BlendFactor::eConstantAlpha;
				break;
			case EBlendFactor::one_minus_constant_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusConstantAlpha;
				break;
			case EBlendFactor::src_alpha_saturate:
				src_blend_factor = vk::BlendFactor::eSrcAlphaSaturate;
				break;
			case EBlendFactor::src_one_color:
				src_blend_factor = vk::BlendFactor::eSrc1Color;
				break;
			case EBlendFactor::one_minus_src_one_color:
				src_blend_factor = vk::BlendFactor::eOneMinusSrc1Color;
				break;
			case EBlendFactor::src_one_alpha:
				src_blend_factor = vk::BlendFactor::eSrc1Alpha;
				break;
			case EBlendFactor::one_minus_src_one_alpha:
				src_blend_factor = vk::BlendFactor::eOneMinusSrc1Alpha;
				break;
		}

		switch (blend.dst) {
			case EBlendFactor::zero:
				dst_blend_factor = vk::BlendFactor::eZero;
				break;
			case EBlendFactor::one:
				dst_blend_factor = vk::BlendFactor::eOne;
				break;
			case EBlendFactor::src_color:
				dst_blend_factor = vk::BlendFactor::eSrcColor;
				break;
			case EBlendFactor::one_minus_src_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrcColor;
				break;
			case EBlendFactor::dst_color:
				dst_blend_factor = vk::BlendFactor::eDstColor;
				break;
			case EBlendFactor::one_minus_dst_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusDstColor;
				break;
			case EBlendFactor::src_alpha:
				dst_blend_factor = vk::BlendFactor::eSrcAlpha;
				break;
			case EBlendFactor::one_minus_src_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrcAlpha;
				break;
			case EBlendFactor::dst_alpha:
				dst_blend_factor = vk::BlendFactor::eDstAlpha;
				break;
			case EBlendFactor::one_minus_dst_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusDstAlpha;
				break;
			case EBlendFactor::constant_color:
				dst_blend_factor = vk::BlendFactor::eConstantColor;
				break;
			case EBlendFactor::one_minus_constant_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusConstantColor;
				break;
			case EBlendFactor::constant_alpha:
				dst_blend_factor = vk::BlendFactor::eConstantAlpha;
				break;
			case EBlendFactor::one_minus_constant_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusConstantAlpha;
				break;
			case EBlendFactor::src_alpha_saturate:
				dst_blend_factor = vk::BlendFactor::eSrcAlphaSaturate;
				break;
			case EBlendFactor::src_one_color:
				dst_blend_factor = vk::BlendFactor::eSrc1Color;
				break;
			case EBlendFactor::one_minus_src_one_color:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrc1Color;
				break;
			case EBlendFactor::src_one_alpha:
				dst_blend_factor = vk::BlendFactor::eSrc1Alpha;
				break;
			case EBlendFactor::one_minus_src_one_alpha:
				dst_blend_factor = vk::BlendFactor::eOneMinusSrc1Alpha;
				break;
		}

		m_ColorBlendAttachment.setAlphaBlendOp(op);
		m_ColorBlendAttachment.setSrcAlphaBlendFactor(src_blend_factor);
		m_ColorBlendAttachment.setDstAlphaBlendFactor(dst_blend_factor);
		return *this;
	}

	auto RenderPassBuilder::set_blend_mask(bool r, bool g, bool b, bool a) -> RenderPassBuilder& {
		vk::ColorComponentFlags flags;
		if (r) flags |= vk::ColorComponentFlagBits::eR;
		if (g) flags |= vk::ColorComponentFlagBits::eG;
		if (b) flags |= vk::ColorComponentFlagBits::eB;
		if (a) flags |= vk::ColorComponentFlagBits::eA;
		m_ColorBlendAttachment.setColorWriteMask(flags);
		return *this;
	}

	auto RenderPassBuilder::disable_blending() -> RenderPassBuilder& {
		m_ColorBlendAttachment.setColorWriteMask(
		    vk::ColorComponentFlagBits::eR |
		    vk::ColorComponentFlagBits::eG |
		    vk::ColorComponentFlagBits::eB |
		    vk::ColorComponentFlagBits::eA);
		m_ColorBlendAttachment.setBlendEnable(vk::False);
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
		return *this;
	}

} // namespace aby::rhi::vulkan
