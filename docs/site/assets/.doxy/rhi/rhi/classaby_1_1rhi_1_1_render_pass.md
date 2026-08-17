

# Class aby::rhi::RenderPass



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**RenderPass**](classaby_1_1rhi_1_1_render_pass.md)



_Use the_ [_**RenderPassBuilder**_](classaby_1_1rhi_1_1_render_pass_builder.md) _to construct this._

* `#include <render-pass.hpp>`





Inherited by the following classes: [aby::rhi::vulkan::RenderPass](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**begin**](#function-begin) () = 0<br>_begin rendering using attachments_  |
| virtual void | [**bind**](#function-bind) () = 0<br>_bind the pipeline, buffers, and any other resources used._  |
|  void | [**clear**](#function-clear) () <br>_The functions below should not be called by the user. only by the renderer backend. these functions must be called during Renderer::on\_begin._  |
| virtual void | [**destroy**](#function-destroy) () = 0<br>_destroy all resources the_ [_**RenderPass**_](classaby_1_1rhi_1_1_render_pass.md) _owns._ |
| virtual void | [**end**](#function-end) () = 0<br>_end rendering using attachments_  |
| virtual void | [**run**](#function-run) () = 0<br>_executes each draw command._  |
| virtual void | [**set\_scissor**](#function-set_scissor) (vec2&lt; float &gt; offset, vec2&lt; float &gt; size) = 0<br>_set the scissor._  |
|  void | [**set\_uniform**](#function-set_uniform-12) (std::string\_view name, T & obj) <br> |
| virtual void | [**set\_uniform**](#function-set_uniform-22) (std::string\_view name, void \* data, size\_t bytes) = 0<br> |
| virtual void | [**set\_viewport**](#function-set_viewport) (vec2&lt; float &gt; size, vec2&lt; float &gt; loc={ 0.f, 0.f }, vec2&lt; float &gt; min\_max\_depth={ 0.f, 1.f }) = 0<br>_set the viewport. In backends like vulkan it will be configured to use loc=0,0 as the top left._  |
|  void | [**submit**](#function-submit) (const [**DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md) & cmd) <br>_Submit a draw cmd to be rendered. This must be called each frame._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::vector&lt; [**DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md) &gt; | [**m\_Commands**](#variable-m_commands)  <br> |




















## Public Functions Documentation




### function begin 

_begin rendering using attachments_ 
```C++
virtual void aby::rhi::RenderPass::begin () = 0
```




<hr>



### function bind 

_bind the pipeline, buffers, and any other resources used._ 
```C++
virtual void aby::rhi::RenderPass::bind () = 0
```




<hr>



### function clear 

_The functions below should not be called by the user. only by the renderer backend. these functions must be called during Renderer::on\_begin._ 
```C++
void aby::rhi::RenderPass::clear () 
```



Clear the list of commands 


        

<hr>



### function destroy 

_destroy all resources the_ [_**RenderPass**_](classaby_1_1rhi_1_1_render_pass.md) _owns._
```C++
virtual void aby::rhi::RenderPass::destroy () = 0
```




<hr>



### function end 

_end rendering using attachments_ 
```C++
virtual void aby::rhi::RenderPass::end () = 0
```




<hr>



### function run 

_executes each draw command._ 
```C++
virtual void aby::rhi::RenderPass::run () = 0
```




<hr>



### function set\_scissor 

_set the scissor._ 
```C++
virtual void aby::rhi::RenderPass::set_scissor (
    vec2< float > offset,
    vec2< float > size
) = 0
```




<hr>



### function set\_uniform [1/2]

```C++
template<typename T>
inline void aby::rhi::RenderPass::set_uniform (
    std::string_view name,
    T & obj
) 
```




<hr>



### function set\_uniform [2/2]

```C++
virtual void aby::rhi::RenderPass::set_uniform (
    std::string_view name,
    void * data,
    size_t bytes
) = 0
```




<hr>



### function set\_viewport 

_set the viewport. In backends like vulkan it will be configured to use loc=0,0 as the top left._ 
```C++
virtual void aby::rhi::RenderPass::set_viewport (
    vec2< float > size,
    vec2< float > loc={ 0.f, 0.f },
    vec2< float > min_max_depth={ 0.f, 1.f }
) = 0
```




<hr>



### function submit 

_Submit a draw cmd to be rendered. This must be called each frame._ 
```C++
void aby::rhi::RenderPass::submit (
    const DrawCmd & cmd
) 
```





**Parameters:**


* `cmd` The draw command. 




        

<hr>
## Protected Attributes Documentation




### variable m\_Commands 

```C++
std::vector<DrawCmd> aby::rhi::RenderPass::m_Commands;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/render-pass.hpp`

