

# Class aby::rhi::vulkan::Renderer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Renderer**](classaby_1_1rhi_1_1vulkan_1_1_renderer.md)








Inherits the following classes: [aby::rhi::Renderer](classaby_1_1rhi_1_1_renderer.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Renderer**](#function-renderer) ([**GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) params) <br> |
|  void | [**add\_pass**](#function-add_pass) (std::shared\_ptr&lt; [**rhi::RenderPass**](classaby_1_1rhi_1_1_render_pass.md) &gt; render\_pass) override<br> |
|  vk::ClearColorValue | [**clear\_color**](#function-clear_color) () const<br> |
|  vk::Format | [**color\_format**](#function-color_format) () <br> |
| virtual void | [**deinit**](#function-deinit) () override<br> |
|  [**DescriptorAllocator**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_allocator.md) & | [**desc\_alloc**](#function-desc_alloc) () <br> |
|  vkb::Device & | [**device**](#function-device) () <br> |
|  size\_t | [**frame\_index**](#function-frame_index) () const<br> |
|  [**GarbageCollector**](classaby_1_1rhi_1_1vulkan_1_1_garbage_collector.md) & | [**gc**](#function-gc) () <br> |
|  [**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* | [**get\_resolve\_attachment**](#function-get_resolve_attachment) ([**rhi::Texture**](classaby_1_1rhi_1_1_texture.md) \* color\_attachment) <br> |
|  const [**GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) & | [**graphics**](#function-graphics) () const<br> |
|  uint32\_t | [**height**](#function-height) () const<br> |
|  bool | [**immediate\_submit**](#function-immediate_submit) (std::function&lt; void(vk::CommandBuffer)&gt; && fn) <br> |
| virtual bool | [**init**](#function-init) (void \* native\_window) override<br> |
|  float | [**max\_sampler\_anisotropy**](#function-max_sampler_anisotropy) () <br> |
| virtual bool | [**on\_begin**](#function-on_begin) () override<br> |
| virtual bool | [**on\_end**](#function-on_end) () override<br> |
|  uint32\_t | [**register\_texture**](#function-register_texture) (ResourceID id, vk::ImageView view, vk::Sampler smapler) <br> |
| virtual void | [**set\_clear\_color**](#function-set_clear_color) (Color color) override<br> |
|  vk::DescriptorSetLayout | [**tex\_desc\_layout**](#function-tex_desc_layout) () <br> |
|  vk::DescriptorSet | [**tex\_desc\_set**](#function-tex_desc_set) () <br> |
|  void | [**update\_texture**](#function-update_texture) (uint32\_t texture\_id, vk::ImageView view, vk::Sampler sampler) <br> |
|  VmaAllocator & | [**vma**](#function-vma) () <br> |
|  uint32\_t | [**width**](#function-width) () const<br> |
| virtual  | [**~Renderer**](#function-renderer) () = default<br> |


## Public Functions inherited from aby::rhi::Renderer

See [aby::rhi::Renderer](classaby_1_1rhi_1_1_renderer.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**add\_pass**](classaby_1_1rhi_1_1_renderer.md#function-add_pass) (std::shared\_ptr&lt; [**RenderPass**](classaby_1_1rhi_1_1_render_pass.md) &gt; render\_pass) = 0<br> |
| virtual void | [**deinit**](classaby_1_1rhi_1_1_renderer.md#function-deinit) () = 0<br> |
| virtual bool | [**init**](classaby_1_1rhi_1_1_renderer.md#function-init) (void \* native\_window) = 0<br> |
| virtual bool | [**on\_begin**](classaby_1_1rhi_1_1_renderer.md#function-on_begin) () = 0<br> |
| virtual bool | [**on\_end**](classaby_1_1rhi_1_1_renderer.md#function-on_end) () = 0<br> |
| virtual void | [**set\_clear\_color**](classaby_1_1rhi_1_1_renderer.md#function-set_clear_color) (Color color) = 0<br> |
| virtual  | [**~Renderer**](classaby_1_1rhi_1_1_renderer.md#function-renderer) () = default<br> |




## Public Static Functions inherited from aby::rhi::Renderer

See [aby::rhi::Renderer](classaby_1_1rhi_1_1_renderer.md)

| Type | Name |
| ---: | :--- |
|  [**Renderer**](classaby_1_1rhi_1_1_renderer.md) \* | [**create**](classaby_1_1rhi_1_1_renderer.md#function-create) (ERenderer backend, const [**GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) & graphics) <br>_Create a renderer for the backend._  |










































## Protected Functions

| Type | Name |
| ---: | :--- |
|  [**ImmediateCommands**](classaby_1_1rhi_1_1vulkan_1_1_immediate_commands.md) & | [**get\_immediate**](#function-get_immediate) () <br> |
|  bool | [**recreate\_swapchain**](#function-recreate_swapchain) () <br> |








## Public Functions Documentation




### function Renderer 

```C++
aby::rhi::vulkan::Renderer::Renderer (
    GraphicsParams params
) 
```




<hr>



### function add\_pass 

```C++
void aby::rhi::vulkan::Renderer::add_pass (
    std::shared_ptr< rhi::RenderPass > render_pass
) override
```




<hr>



### function clear\_color 

```C++
vk::ClearColorValue aby::rhi::vulkan::Renderer::clear_color () const
```




<hr>



### function color\_format 

```C++
vk::Format aby::rhi::vulkan::Renderer::color_format () 
```




<hr>



### function deinit 

```C++
virtual void aby::rhi::vulkan::Renderer::deinit () override
```



Implements [*aby::rhi::Renderer::deinit*](classaby_1_1rhi_1_1_renderer.md#function-deinit)


<hr>



### function desc\_alloc 

```C++
DescriptorAllocator & aby::rhi::vulkan::Renderer::desc_alloc () 
```




<hr>



### function device 

```C++
vkb::Device & aby::rhi::vulkan::Renderer::device () 
```




<hr>



### function frame\_index 

```C++
size_t aby::rhi::vulkan::Renderer::frame_index () const
```




<hr>



### function gc 

```C++
GarbageCollector & aby::rhi::vulkan::Renderer::gc () 
```




<hr>



### function get\_resolve\_attachment 

```C++
rhi::Texture * aby::rhi::vulkan::Renderer::get_resolve_attachment (
    rhi::Texture * color_attachment
) 
```




<hr>



### function graphics 

```C++
const GraphicsParams & aby::rhi::vulkan::Renderer::graphics () const
```




<hr>



### function height 

```C++
uint32_t aby::rhi::vulkan::Renderer::height () const
```




<hr>



### function immediate\_submit 

```C++
bool aby::rhi::vulkan::Renderer::immediate_submit (
    std::function< void(vk::CommandBuffer)> && fn
) 
```




<hr>



### function init 

```C++
virtual bool aby::rhi::vulkan::Renderer::init (
    void * native_window
) override
```



Implements [*aby::rhi::Renderer::init*](classaby_1_1rhi_1_1_renderer.md#function-init)


<hr>



### function max\_sampler\_anisotropy 

```C++
float aby::rhi::vulkan::Renderer::max_sampler_anisotropy () 
```




<hr>



### function on\_begin 

```C++
virtual bool aby::rhi::vulkan::Renderer::on_begin () override
```



Implements [*aby::rhi::Renderer::on\_begin*](classaby_1_1rhi_1_1_renderer.md#function-on_begin)


<hr>



### function on\_end 

```C++
virtual bool aby::rhi::vulkan::Renderer::on_end () override
```



Implements [*aby::rhi::Renderer::on\_end*](classaby_1_1rhi_1_1_renderer.md#function-on_end)


<hr>



### function register\_texture 

```C++
uint32_t aby::rhi::vulkan::Renderer::register_texture (
    ResourceID id,
    vk::ImageView view,
    vk::Sampler smapler
) 
```




<hr>



### function set\_clear\_color 

```C++
virtual void aby::rhi::vulkan::Renderer::set_clear_color (
    Color color
) override
```



Implements [*aby::rhi::Renderer::set\_clear\_color*](classaby_1_1rhi_1_1_renderer.md#function-set_clear_color)


<hr>



### function tex\_desc\_layout 

```C++
vk::DescriptorSetLayout aby::rhi::vulkan::Renderer::tex_desc_layout () 
```




<hr>



### function tex\_desc\_set 

```C++
vk::DescriptorSet aby::rhi::vulkan::Renderer::tex_desc_set () 
```




<hr>



### function update\_texture 

```C++
void aby::rhi::vulkan::Renderer::update_texture (
    uint32_t texture_id,
    vk::ImageView view,
    vk::Sampler sampler
) 
```




<hr>



### function vma 

```C++
VmaAllocator & aby::rhi::vulkan::Renderer::vma () 
```




<hr>



### function width 

```C++
uint32_t aby::rhi::vulkan::Renderer::width () const
```




<hr>



### function ~Renderer 

```C++
virtual aby::rhi::vulkan::Renderer::~Renderer () = default
```



Implements [*aby::rhi::Renderer::~Renderer*](classaby_1_1rhi_1_1_renderer.md#function-renderer)


<hr>
## Protected Functions Documentation




### function get\_immediate 

```C++
ImmediateCommands & aby::rhi::vulkan::Renderer::get_immediate () 
```




<hr>



### function recreate\_swapchain 

```C++
bool aby::rhi::vulkan::Renderer::recreate_swapchain () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-renderer.hpp`

