#include "backends/vulkan/vulkan-pipeline.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-common.hpp"
#include "context.hpp"
#include <array>

namespace aby::rhi::vulkan {

    Pipeline::Pipeline(vk::Pipeline pipeline, vk::PipelineLayout layout, const std::vector<vk::DescriptorSet>& sets) :
        m_Pipeline(pipeline),
        m_Layout(layout),
        m_DescriptorSets(sets)
    {

    } 
        
    auto Pipeline::bind(vk::CommandBuffer cmd, vk::PipelineBindPoint point) -> void {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
        vkCmdBindPipeline(cmd, static_cast<VkPipelineBindPoint>(point), m_Pipeline);

        for (auto& set : m_DescriptorSets) {
            vkCmdBindDescriptorSets(
                cmd,
                static_cast<VkPipelineBindPoint>(point),
                m_Layout,
                0, /* first set */
                m_DescriptorSets.size(),
                vkcast(m_DescriptorSets.data()),
                0,      /* dynamic offset count */
                nullptr /* dynamic offsets */
            );
        }
    }
    
    auto Pipeline::destroy() -> void {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
        vkDestroyPipelineLayout(r->device(), m_Layout, allocator());
        vkDestroyPipeline(r->device(), m_Pipeline, allocator());
    }

    Pipeline::operator vk::Pipeline() {
        return m_Pipeline;
    } 

    Pipeline::operator VkPipeline() {
        return m_Pipeline;
    }


}

namespace aby::rhi::vulkan {

    PipelineBuilder::PipelineBuilder() {
        clear();
    } 

    auto PipelineBuilder::clear() -> void {
        m_Layout                = vk::PipelineLayout();
        m_InputAssembly         = vk::PipelineInputAssemblyStateCreateInfo();
        m_Rasterizer            = vk::PipelineRasterizationStateCreateInfo();
        m_ColorBlendAttachment  = vk::PipelineColorBlendAttachmentState();
        m_Multisampling         = vk::PipelineMultisampleStateCreateInfo();
        m_DepthStencil          = vk::PipelineDepthStencilStateCreateInfo();
        m_RenderInfo            = vk::PipelineRenderingCreateInfo();
        m_ColorAttachmentFormat = vk::Format::eUndefined;
        m_ShaderStages.clear();
    }    

    auto PipelineBuilder::build() -> std::unique_ptr<Pipeline> {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

        vk::PipelineVertexInputStateCreateInfo vertex_input_info(
            vk::PipelineVertexInputStateCreateFlags(),
            m_VertexInputBindings.size(),
            m_VertexInputBindings.data(),
            m_VertexAttributes.size(),
            m_VertexAttributes.data()
        );

        vk::PipelineTessellationStateCreateInfo tesselation_input_info;
        vk::PipelineViewportStateCreateInfo viewport_state(
            vk::PipelineViewportStateCreateFlags{},
            1,       /* viewport count */
            nullptr, /* viewports */
            1,       /* scissor count */
            nullptr  /* scissors */
        );

        auto dynamic_states = std::to_array<vk::DynamicState>({
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        });

        vk::PipelineDynamicStateCreateInfo dynamic_state_info(
            vk::PipelineDynamicStateCreateFlags{},
            dynamic_states.size(),
            dynamic_states.data()
        );
        vk::PipelineColorBlendStateCreateInfo color_blend_state(
            vk::PipelineColorBlendStateCreateFlags(),
            vk::False,
            vk::LogicOp::eCopy,
            1,
            &m_ColorBlendAttachment
        );

        if (!m_UniformBindings.empty()) {
            std::vector<vk::DescriptorSetLayoutBinding> bindings;
            bindings.reserve(m_UniformBindings.size());
            for (auto& [name, binding] : m_UniformBindings) {   
                bindings.push_back(binding);
            }
            vk::DescriptorSetLayoutCreateInfo descriptor_layout_ci(
                vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
                bindings.size(),
                bindings.data()
            );
            vk::DescriptorSetLayout layout;
            vkCreateDescriptorSetLayout(
                r->device(),
                descriptor_layout_ci,
                allocator(),
                vkcast(layout)
            );
            m_DescriptorSetLayouts.push_back(layout);

            auto& desc_alloc = r->desc_alloc();
            m_DescriptorSets.push_back(desc_alloc.alloc(layout));
        }

        vk::PipelineLayoutCreateInfo layout_create_info(
            vk::PipelineLayoutCreateFlags(),
            m_DescriptorSetLayouts.size(),
            m_DescriptorSetLayouts.data(),
            0,       /* push constant ranges count */
            nullptr  /*  push constant ranges      */
        );

        vkassert(vkCreatePipelineLayout(
            r->device(),
            vkcast(layout_create_info),
            allocator(),
            vkcast(m_Layout)
        ), "failed to create pipeline layout");

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
            m_Layout,
            vk::RenderPass{},
            0, /* subpass */
            vk::Pipeline{},
            0, /* base pipeline index */
            static_cast<void*>(&m_RenderInfo)
        );


        vk::Pipeline pipeline;
        
        vkassert(vkCreateGraphicsPipelines(
            r->device(),
            VK_NULL_HANDLE, /* pipeline cache */
            1,              /* pipeline count */
            vkcast(create_info), 
            allocator(),
            vkcast(pipeline)
        ), "failed to create graphics pipeline");

        return std::make_unique<Pipeline>(pipeline, m_Layout, m_DescriptorSets);
    }   


    auto PipelineBuilder::add_shader(EShader type, vk::ShaderModule shader) -> PipelineBuilder& {
        vk::ShaderStageFlagBits stage;

        switch (type) {
            case EShader::vert: stage = vk::ShaderStageFlagBits::eVertex;   break;
            case EShader::frag: stage = vk::ShaderStageFlagBits::eFragment; break;
            case EShader::comp: stage = vk::ShaderStageFlagBits::eCompute;  break;
            case EShader::geom: stage = vk::ShaderStageFlagBits::eGeometry; break;
            default: 
                break;
        }

        vk::PipelineShaderStageCreateInfo create_info(
            vk::PipelineShaderStageCreateFlags(),
            stage,
            shader,
            "main"
        );

        m_ShaderStages.push_back(create_info);

        return *this;
    }

    auto PipelineBuilder::add_descriptor_set_layout(vk::DescriptorSetLayout layout) -> PipelineBuilder& {
        m_DescriptorSetLayouts.push_back(layout);
        return *this;
    }

    auto PipelineBuilder::add_vertex_type(uint32_t binding, size_t bytes_of_vertex_type) -> PipelineBuilder& {
        m_VertexInputBindings.push_back(vk::VertexInputBindingDescription(
            binding,
            bytes_of_vertex_type,
            vk::VertexInputRate::eVertex
        ));
        return *this;
    }


    auto PipelineBuilder::add_vertex_field(uint32_t location, uint32_t binding, vk::Format format, size_t offset_of) -> PipelineBuilder& {
        m_VertexAttributes.push_back(vk::VertexInputAttributeDescription(
            location,
            binding,
            format,
            offset_of
        ));
        return *this;
    }
        
    auto PipelineBuilder::add_uniform(std::string_view name, uint32_t binding, vk::ShaderStageFlags stage) -> PipelineBuilder& {
        m_UniformBindings[std::string(name)] = vk::DescriptorSetLayoutBinding(
            binding,
            vk::DescriptorType::eUniformBuffer,
            1, /* descriptor count */
            stage
        );
        return *this;
    }

    auto PipelineBuilder::set_topology(vk::PrimitiveTopology topology) -> PipelineBuilder& {
        m_InputAssembly.setTopology(topology);
        m_InputAssembly.setPrimitiveRestartEnable(vk::False);
        return *this;
    }
    
    auto PipelineBuilder::set_polygon_mode(vk::PolygonMode mode) -> PipelineBuilder& {
        m_Rasterizer.setPolygonMode(mode);
        m_Rasterizer.setLineWidth(1.f);
        return *this;
    }
    
    auto PipelineBuilder::set_cull_mode(vk::CullModeFlags mode, vk::FrontFace front_face) -> PipelineBuilder& {
        m_Rasterizer.setCullMode(mode);
        m_Rasterizer.setFrontFace(front_face);
        return *this;
    }
    
    auto PipelineBuilder::set_color_attachment_format(vk::Format format) -> PipelineBuilder& {
        m_ColorAttachmentFormat = format;
        m_RenderInfo.setColorAttachmentCount(1);
        m_RenderInfo.setPColorAttachmentFormats(&m_ColorAttachmentFormat);
        return *this;
    }
    
    auto PipelineBuilder::set_depth_format(vk::Format format) -> PipelineBuilder& {
        m_RenderInfo.setDepthAttachmentFormat(format);
        return *this;
    }

    auto PipelineBuilder::disable_multisampling() -> PipelineBuilder& {
        m_Multisampling.setSampleShadingEnable(vk::False);
        m_Multisampling.setRasterizationSamples(vk::SampleCountFlagBits::e1);
        m_Multisampling.setMinSampleShading(1.f);
        m_Multisampling.setPSampleMask(nullptr);
        m_Multisampling.setAlphaToCoverageEnable(vk::False);
        m_Multisampling.setAlphaToOneEnable(vk::False);
        return *this;
    }

    auto PipelineBuilder::disable_blending() -> PipelineBuilder& {
        m_ColorBlendAttachment.setColorWriteMask(
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA 
        );
        m_ColorBlendAttachment.setBlendEnable(vk::False);
        return *this;
    }

    auto PipelineBuilder::disable_depthtest() -> PipelineBuilder& {
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

}