
# Class Hierarchy

This inheritance list is sorted roughly, but not completely, alphabetically:


* **class** [**aby::rhi::Buffer**](classaby_1_1rhi_1_1_buffer.md)     
    * **class** [**aby::rhi::IndexBuffer**](classaby_1_1rhi_1_1_index_buffer.md)     
        * **class** [**aby::rhi::vulkan::IndexBuffer**](classaby_1_1rhi_1_1vulkan_1_1_index_buffer.md) 
    * **class** [**aby::rhi::VertexBuffer**](classaby_1_1rhi_1_1_vertex_buffer.md)     
        * **class** [**aby::rhi::vulkan::VertexBuffer**](classaby_1_1rhi_1_1vulkan_1_1_vertex_buffer.md) 
* **class** [**aby::rhi::Context**](classaby_1_1rhi_1_1_context.md) 
* **class** [**aby::rhi::IInterface**](classaby_1_1rhi_1_1_i_interface.md) _Base class for interfaces providing a common subset of debugging information._     
    * **class** [**aby::rhi::IAllocator**](classaby_1_1rhi_1_1_i_allocator.md) [_**IAllocator**_](classaby_1_1rhi_1_1_i_allocator.md) _interface for alloc/free/realloc._    
        * **class** [**aby::rhi::DefaultAllocator**](classaby_1_1rhi_1_1_default_allocator.md) 
    * **class** [**aby::rhi::IFileIO**](classaby_1_1rhi_1_1_i_file_i_o.md) [_**IFileIO**_](classaby_1_1rhi_1_1_i_file_i_o.md) _interface for reading/writing data. Used for reading/writing to/from shaders/textures._    
        * **class** [**aby::rhi::DefaultFileIO**](classaby_1_1rhi_1_1_default_file_i_o.md) 
    * **class** [**aby::rhi::IJobSystem**](classaby_1_1rhi_1_1_i_job_system.md)     
        * **class** [**aby::rhi::DefaultJobSystem**](classaby_1_1rhi_1_1_default_job_system.md) 
    * **class** [**aby::rhi::ILogger**](classaby_1_1rhi_1_1_i_logger.md) [_**ILogger**_](classaby_1_1rhi_1_1_i_logger.md) _message for passing pre-formatted messages to the application._    
        * **class** [**aby::rhi::DefaultLogger**](classaby_1_1rhi_1_1_default_logger.md) 
* **class** [**aby::rhi::DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md) _Draw Cmd ares to be submitted to a created render pass each frame._ 
* **class** [**aby::rhi::RenderPass**](classaby_1_1rhi_1_1_render_pass.md) _Use the_ [_**RenderPassBuilder**_](classaby_1_1rhi_1_1_render_pass_builder.md) _to construct this._    
    * **class** [**aby::rhi::vulkan::RenderPass**](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md) 
* **class** [**aby::rhi::RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md)     
    * **class** [**aby::rhi::vulkan::RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) 
* **class** [**aby::rhi::Renderer**](classaby_1_1rhi_1_1_renderer.md)     
    * **class** [**aby::rhi::vulkan::Renderer**](classaby_1_1rhi_1_1vulkan_1_1_renderer.md) 
* **class** [**aby::rhi::Resource**](classaby_1_1rhi_1_1_resource.md) _A resource is a handle to the resource data contained with the_ [_**Context**_](classaby_1_1rhi_1_1_context.md) _. This is used for loading data asynchronously while still being able to give out valid usable IDs._    
    * **class** [**aby::rhi::ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md) [_**ResourcePtr**_](classaby_1_1rhi_1_1_resource_ptr.md) _class that abstracts the synchronization mechanisms from the user._
    * **class** [**aby::rhi::ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md) [_**ResourcePtr**_](classaby_1_1rhi_1_1_resource_ptr.md) _class that abstracts the synchronization mechanisms from the user._
    * **class** [**aby::rhi::ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md) [_**ResourcePtr**_](classaby_1_1rhi_1_1_resource_ptr.md) _class that abstracts the synchronization mechanisms from the user._
* **class** [**aby::rhi::ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md) _A resource container that owns the resources and performs cleanup._ 
* **class** [**aby::rhi::Shader**](classaby_1_1rhi_1_1_shader.md)     
    * **class** [**aby::rhi::vulkan::Shader**](classaby_1_1rhi_1_1vulkan_1_1_shader.md) 
* **class** [**aby::rhi::Texture**](classaby_1_1rhi_1_1_texture.md)     
    * **class** [**aby::rhi::vulkan::Texture**](classaby_1_1rhi_1_1vulkan_1_1_texture.md) 
* **class** [**aby::rhi::VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) 
* **class** [**aby::rhi::detail::ScopedTimer**](classaby_1_1rhi_1_1detail_1_1_scoped_timer.md) 
* **class** [**aby::rhi::vulkan::Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) 
* **class** [**aby::rhi::vulkan::DescriptorAllocator**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_allocator.md) 
* **class** [**aby::rhi::vulkan::DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md) 
* **class** [**aby::rhi::vulkan::Frame**](classaby_1_1rhi_1_1vulkan_1_1_frame.md) 
* **class** [**aby::rhi::vulkan::Frames**](classaby_1_1rhi_1_1vulkan_1_1_frames.md) 
* **class** [**aby::rhi::vulkan::GarbageCollector**](classaby_1_1rhi_1_1vulkan_1_1_garbage_collector.md) 
* **class** [**aby::rhi::vulkan::Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) 
* **class** [**aby::rhi::vulkan::ImmediateCommands**](classaby_1_1rhi_1_1vulkan_1_1_immediate_commands.md) 
* **class** [**aby::rhi::vulkan::Pipeline**](classaby_1_1rhi_1_1vulkan_1_1_pipeline.md) 
* **struct** [**aby::rhi::Blend**](structaby_1_1rhi_1_1_blend.md) 
* **struct** [**aby::rhi::ContextParams**](structaby_1_1rhi_1_1_context_params.md) 
* **struct** [**aby::rhi::DefaultJobQueue**](structaby_1_1rhi_1_1_default_job_queue.md) 
* **struct** [**aby::rhi::GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) 
* **struct** [**aby::rhi::Shader::PathData**](structaby_1_1rhi_1_1_shader_1_1_path_data.md) 
* **struct** [**aby::rhi::TextureParams**](structaby_1_1rhi_1_1_texture_params.md) 
* **struct** [**aby::rhi::UniformBufferDesc**](structaby_1_1rhi_1_1_uniform_buffer_desc.md) 
* **struct** [**aby::rhi::VertexAttributeDesc**](structaby_1_1rhi_1_1_vertex_attribute_desc.md) 
* **struct** [**aby::rhi::VertexInput**](structaby_1_1rhi_1_1_vertex_input.md) 
* **struct** [**aby::rhi::VertexShaderDesc**](structaby_1_1rhi_1_1_vertex_shader_desc.md) 
* **struct** [**aby::rhi::vulkan::PoolSizeRatio**](structaby_1_1rhi_1_1vulkan_1_1_pool_size_ratio.md) 
* **struct** [**aby::rhi::vulkan::Uniform**](structaby_1_1rhi_1_1vulkan_1_1_uniform.md) 
* **struct** [**aby::rhi::VertexInputDescriptionBuilder::member\_pointer\_traits**](structaby_1_1rhi_1_1_vertex_input_description_builder_1_1member__pointer__traits.md) 
* **struct** [**aby::rhi::VertexInputDescriptionBuilder::member\_pointer\_traits&lt; Member Class::\* &gt;**](structaby_1_1rhi_1_1_vertex_input_description_builder_1_1member__pointer__traits_3_01_member_01_class_1_1_5_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EAllocation, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_allocation_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EAntiAliasing, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_anti_aliasing_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EBlendFactor, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_blend_factor_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EBlendOp, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_blend_op_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EChannels, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_channels_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ECompareOp, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_compare_op_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ECullMode, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_cull_mode_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EFiltering, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_filtering_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EFormat, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_format_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EFrontFace, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_front_face_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EJobPriority, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_job_priority_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EJobQueue, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_job_queue_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ELogLevel, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_log_level_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EPolygonMode, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_polygon_mode_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ERenderer, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_renderer_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ERepeatMode, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_repeat_mode_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EResource, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_resource_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EResourceState, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_resource_state_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EShader, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_shader_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ETextureUsage, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_texture_usage_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ETopology, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_topology_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::EWindow, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_e_window_00_01char_01_4.md) 
* **struct** [**std::formatter&lt; aby::rhi::ResourceID, char &gt;**](structstd_1_1formatter_3_01aby_1_1rhi_1_1_resource_i_d_00_01char_01_4.md) 

