

# Class aby::rhi::vulkan::RenderPass



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**RenderPass**](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md)








Inherits the following classes: [aby::rhi::RenderPass](classaby_1_1rhi_1_1_render_pass.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RenderPass**](#function-renderpass) (std::unique\_ptr&lt; [**Pipeline**](classaby_1_1rhi_1_1vulkan_1_1_pipeline.md) &gt; pipeline, const std::vector&lt; [**Resource**](classaby_1_1rhi_1_1_resource.md) &gt; & shaders, const std::unordered\_map&lt; std::string, [**Uniform**](structaby_1_1rhi_1_1vulkan_1_1_uniform.md) &gt; & uniforms, const std::vector&lt; [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* &gt; & color\_attachments, const std::vector&lt; [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* &gt; & resolve\_attachments, [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* present\_attachment) <br> |
| virtual void | [**begin**](#function-begin) () override<br>_begin rendering using attachments_  |
| virtual void | [**bind**](#function-bind) () override<br>_bind the pipeline, buffers, and any other resources used._  |
|  std::vector&lt; [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* &gt; & | [**color\_attachments**](#function-color_attachments) () <br> |
| virtual void | [**destroy**](#function-destroy) () override<br>_destroy all resources the_ [_**RenderPass**_](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md) _owns._ |
| virtual void | [**end**](#function-end) () override<br>_end rendering using attachments_  |
|  bool | [**is\_present**](#function-is_present) () const<br> |
|  [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* | [**present\_attachment**](#function-present_attachment) () <br> |
|  std::vector&lt; [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* &gt; & | [**resolve\_attachments**](#function-resolve_attachments) () <br> |
| virtual void | [**run**](#function-run) () override<br>_executes each draw command._  |
|  [**RenderPass**](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md) & | [**set\_bind\_point**](#function-set_bind_point) (vk::PipelineBindPoint point) <br> |
|  [**RenderPass**](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md) & | [**set\_cmd\_buffer**](#function-set_cmd_buffer) (vk::CommandBuffer cmd) <br> |
| virtual void | [**set\_scissor**](#function-set_scissor) (vec2&lt; float &gt; offset, vec2&lt; float &gt; size) override<br>_set the scissor._  |
| virtual void | [**set\_uniform**](#function-set_uniform) (std::string\_view name, void \* data, size\_t bytes) override<br> |
| virtual void | [**set\_viewport**](#function-set_viewport) (vec2&lt; float &gt; size, vec2&lt; float &gt; loc={ 0.f, 0.f }, vec2&lt; float &gt; min\_max\_depth={ 0.f, 1.f }) override<br>_set the viewport. In backends like vulkan it will be configured to use loc=0,0 as the top left._  |


## Public Functions inherited from aby::rhi::RenderPass

See [aby::rhi::RenderPass](classaby_1_1rhi_1_1_render_pass.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**begin**](classaby_1_1rhi_1_1_render_pass.md#function-begin) () = 0<br>_begin rendering using attachments_  |
| virtual void | [**bind**](classaby_1_1rhi_1_1_render_pass.md#function-bind) () = 0<br>_bind the pipeline, buffers, and any other resources used._  |
|  void | [**clear**](classaby_1_1rhi_1_1_render_pass.md#function-clear) () <br>_The functions below should not be called by the user. only by the renderer backend. these functions must be called during Renderer::on\_begin._  |
| virtual void | [**destroy**](classaby_1_1rhi_1_1_render_pass.md#function-destroy) () = 0<br>_destroy all resources the_ [_**RenderPass**_](classaby_1_1rhi_1_1_render_pass.md) _owns._ |
| virtual void | [**end**](classaby_1_1rhi_1_1_render_pass.md#function-end) () = 0<br>_end rendering using attachments_  |
| virtual void | [**run**](classaby_1_1rhi_1_1_render_pass.md#function-run) () = 0<br>_executes each draw command._  |
| virtual void | [**set\_scissor**](classaby_1_1rhi_1_1_render_pass.md#function-set_scissor) (vec2&lt; float &gt; offset, vec2&lt; float &gt; size) = 0<br>_set the scissor._  |
|  void | [**set\_uniform**](classaby_1_1rhi_1_1_render_pass.md#function-set_uniform-12) (std::string\_view name, T & obj) <br> |
| virtual void | [**set\_uniform**](classaby_1_1rhi_1_1_render_pass.md#function-set_uniform-22) (std::string\_view name, void \* data, size\_t bytes) = 0<br> |
| virtual void | [**set\_viewport**](classaby_1_1rhi_1_1_render_pass.md#function-set_viewport) (vec2&lt; float &gt; size, vec2&lt; float &gt; loc={ 0.f, 0.f }, vec2&lt; float &gt; min\_max\_depth={ 0.f, 1.f }) = 0<br>_set the viewport. In backends like vulkan it will be configured to use loc=0,0 as the top left._  |
|  void | [**submit**](classaby_1_1rhi_1_1_render_pass.md#function-submit) (const [**DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md) & cmd) <br>_Submit a draw cmd to be rendered. This must be called each frame._  |
















## Protected Attributes inherited from aby::rhi::RenderPass

See [aby::rhi::RenderPass](classaby_1_1rhi_1_1_render_pass.md)

| Type | Name |
| ---: | :--- |
|  std::vector&lt; [**DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md) &gt; | [**m\_Commands**](classaby_1_1rhi_1_1_render_pass.md#variable-m_commands)  <br> |






































## Public Functions Documentation




### function RenderPass 

```C++
aby::rhi::vulkan::RenderPass::RenderPass (
    std::unique_ptr< Pipeline > pipeline,
    const std::vector< Resource > & shaders,
    const std::unordered_map< std::string, Uniform > & uniforms,
    const std::vector< rhi::Texture * > & color_attachments,
    const std::vector< rhi::Texture * > & resolve_attachments,
    rhi::Texture * present_attachment
) 
```




<hr>



### function begin 

_begin rendering using attachments_ 
```C++
virtual void aby::rhi::vulkan::RenderPass::begin () override
```



Implements [*aby::rhi::RenderPass::begin*](classaby_1_1rhi_1_1_render_pass.md#function-begin)


<hr>



### function bind 

_bind the pipeline, buffers, and any other resources used._ 
```C++
virtual void aby::rhi::vulkan::RenderPass::bind () override
```



Implements [*aby::rhi::RenderPass::bind*](classaby_1_1rhi_1_1_render_pass.md#function-bind)


<hr>



### function color\_attachments 

```C++
std::vector< rhi::Texture * > & aby::rhi::vulkan::RenderPass::color_attachments () 
```




<hr>



### function destroy 

_destroy all resources the_ [_**RenderPass**_](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md) _owns._
```C++
virtual void aby::rhi::vulkan::RenderPass::destroy () override
```



Implements [*aby::rhi::RenderPass::destroy*](classaby_1_1rhi_1_1_render_pass.md#function-destroy)


<hr>



### function end 

_end rendering using attachments_ 
```C++
virtual void aby::rhi::vulkan::RenderPass::end () override
```



Implements [*aby::rhi::RenderPass::end*](classaby_1_1rhi_1_1_render_pass.md#function-end)


<hr>



### function is\_present 

```C++
bool aby::rhi::vulkan::RenderPass::is_present () const
```




<hr>



### function present\_attachment 

```C++
rhi::Texture * aby::rhi::vulkan::RenderPass::present_attachment () 
```




<hr>



### function resolve\_attachments 

```C++
std::vector< rhi::Texture * > & aby::rhi::vulkan::RenderPass::resolve_attachments () 
```




<hr>



### function run 

_executes each draw command._ 
```C++
virtual void aby::rhi::vulkan::RenderPass::run () override
```



Implements [*aby::rhi::RenderPass::run*](classaby_1_1rhi_1_1_render_pass.md#function-run)


<hr>



### function set\_bind\_point 

```C++
RenderPass & aby::rhi::vulkan::RenderPass::set_bind_point (
    vk::PipelineBindPoint point
) 
```




<hr>



### function set\_cmd\_buffer 

```C++
RenderPass & aby::rhi::vulkan::RenderPass::set_cmd_buffer (
    vk::CommandBuffer cmd
) 
```




<hr>



### function set\_scissor 

_set the scissor._ 
```C++
virtual void aby::rhi::vulkan::RenderPass::set_scissor (
    vec2< float > offset,
    vec2< float > size
) override
```



Implements [*aby::rhi::RenderPass::set\_scissor*](classaby_1_1rhi_1_1_render_pass.md#function-set_scissor)


<hr>



### function set\_uniform 

```C++
virtual void aby::rhi::vulkan::RenderPass::set_uniform (
    std::string_view name,
    void * data,
    size_t bytes
) override
```



Implements [*aby::rhi::RenderPass::set\_uniform*](classaby_1_1rhi_1_1_render_pass.md#function-set_uniform-22)


<hr>



### function set\_viewport 

_set the viewport. In backends like vulkan it will be configured to use loc=0,0 as the top left._ 
```C++
virtual void aby::rhi::vulkan::RenderPass::set_viewport (
    vec2< float > size,
    vec2< float > loc={ 0.f, 0.f },
    vec2< float > min_max_depth={ 0.f, 1.f }
) override
```



Implements [*aby::rhi::RenderPass::set\_viewport*](classaby_1_1rhi_1_1_render_pass.md#function-set_viewport)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-render-pass.hpp`

