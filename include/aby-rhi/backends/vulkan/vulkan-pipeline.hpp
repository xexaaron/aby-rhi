#pragma once
#include "common.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>
#include <unordered_map>

namespace aby::rhi::vulkan {

    class Pipeline {
    public:
        Pipeline(vk::Pipeline pipeline, vk::PipelineLayout layout, const std::vector<vk::DescriptorSet>& sets); 
        
        auto bind(vk::CommandBuffer cmd, vk::PipelineBindPoint point) -> void;
        auto destroy() -> void;

        explicit operator vk::Pipeline(); 
        explicit operator VkPipeline();
    private:
        vk::Pipeline       m_Pipeline;
        vk::PipelineLayout m_Layout;
        std::vector<vk::DescriptorSet> m_DescriptorSets;
    };


    class PipelineBuilder {
    public:
        PipelineBuilder();
        
        auto clear() -> void;    
        auto build() -> std::unique_ptr<Pipeline>;
        
        auto add_shader(EShader type, vk::ShaderModule shader) -> PipelineBuilder&;
        auto add_descriptor_set_layout(vk::DescriptorSetLayout layout) -> PipelineBuilder&;
        auto add_vertex_field(uint32_t location, uint32_t binding, vk::Format format, size_t offset_of) -> PipelineBuilder&;
        auto add_vertex_type(uint32_t binding, size_t bytes_of_vertex_type) -> PipelineBuilder&;
        auto add_uniform(std::string_view name, uint32_t binding, vk::ShaderStageFlags stage) -> PipelineBuilder&;

        auto set_topology(vk::PrimitiveTopology topology) -> PipelineBuilder&;
        auto set_polygon_mode(vk::PolygonMode mode) -> PipelineBuilder&;
        auto set_cull_mode(vk::CullModeFlags mode, vk::FrontFace front_face) -> PipelineBuilder&;
        auto set_color_attachment_format(vk::Format format) -> PipelineBuilder&;
        auto set_depth_format(vk::Format format) -> PipelineBuilder&;

        auto disable_multisampling() -> PipelineBuilder&;
        auto disable_blending() -> PipelineBuilder&;
        auto disable_depthtest() -> PipelineBuilder&;
    private:
        vk::PipelineLayout                               m_Layout;
        vk::PipelineInputAssemblyStateCreateInfo         m_InputAssembly;
        vk::PipelineRasterizationStateCreateInfo         m_Rasterizer;
        vk::PipelineColorBlendAttachmentState            m_ColorBlendAttachment;
        vk::PipelineMultisampleStateCreateInfo           m_Multisampling;
        vk::PipelineDepthStencilStateCreateInfo          m_DepthStencil;
        vk::PipelineRenderingCreateInfo                  m_RenderInfo;
        vk::Format                                       m_ColorAttachmentFormat;  
        std::vector<vk::VertexInputBindingDescription>   m_VertexInputBindings;
        std::vector<vk::VertexInputAttributeDescription> m_VertexAttributes;
        std::vector<vk::PipelineShaderStageCreateInfo>   m_ShaderStages;
        std::vector<vk::DescriptorSetLayout>             m_DescriptorSetLayouts;
        std::vector<vk::DescriptorSet>                   m_DescriptorSets;
        std::unordered_map<std::string, vk::DescriptorSetLayoutBinding> m_UniformBindings;
    };


}