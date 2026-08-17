

# Namespace aby::rhi::vulkan



[**Namespace List**](namespaces.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) <br> |
| class | [**DescriptorAllocator**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_allocator.md) <br> |
| class | [**DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md) <br> |
| class | [**Frame**](classaby_1_1rhi_1_1vulkan_1_1_frame.md) <br> |
| class | [**Frames**](classaby_1_1rhi_1_1vulkan_1_1_frames.md) <br> |
| class | [**GarbageCollector**](classaby_1_1rhi_1_1vulkan_1_1_garbage_collector.md) <br> |
| class | [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) <br> |
| class | [**ImmediateCommands**](classaby_1_1rhi_1_1vulkan_1_1_immediate_commands.md) <br> |
| class | [**IndexBuffer**](classaby_1_1rhi_1_1vulkan_1_1_index_buffer.md) <br> |
| class | [**Pipeline**](classaby_1_1rhi_1_1vulkan_1_1_pipeline.md) <br> |
| struct | [**PoolSizeRatio**](structaby_1_1rhi_1_1vulkan_1_1_pool_size_ratio.md) <br> |
| class | [**RenderPass**](classaby_1_1rhi_1_1vulkan_1_1_render_pass.md) <br> |
| class | [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) <br> |
| class | [**Renderer**](classaby_1_1rhi_1_1vulkan_1_1_renderer.md) <br> |
| class | [**Shader**](classaby_1_1rhi_1_1vulkan_1_1_shader.md) <br> |
| class | [**Texture**](classaby_1_1rhi_1_1vulkan_1_1_texture.md) <br> |
| struct | [**Uniform**](structaby_1_1rhi_1_1vulkan_1_1_uniform.md) <br> |
| class | [**VertexBuffer**](classaby_1_1rhi_1_1vulkan_1_1_vertex_buffer.md) <br> |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  size\_t | [**MAX\_FRAMES\_IN\_FLIGHT**](#variable-max_frames_in_flight)   = `2`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  VkAllocationCallbacks \* | [**allocator**](#function-allocator) () <br> |
|  bool | [**create\_surface**](#function-create_surface) (void \* native\_window, VkInstance instance, VkSurfaceKHR \* surface) <br> |
|  vk::BlendFactor | [**eblendfactor\_to\_vkblendfactor**](#function-eblendfactor_to_vkblendfactor) (EBlendFactor blend\_factor) <br> |
|  vk::BlendOp | [**eblendop\_to\_vkblendop**](#function-eblendop_to_vkblendop) (EBlendOp blend\_op) <br> |
|  vk::CompareOp | [**ecompareop\_to\_vkcompareop**](#function-ecompareop_to_vkcompareop) (ECompareOp compare\_op) <br> |
|  vk::CullModeFlags | [**ecullmode\_to\_vkcullmode**](#function-ecullmode_to_vkcullmode) (ECullMode cull\_mode) <br> |
|  std::pair&lt; vk::Filter, vk::SamplerMipmapMode &gt; | [**efiltering\_to\_vkfilter**](#function-efiltering_to_vkfilter) (EFiltering filtering) <br> |
|  vk::Format | [**eformat\_to\_vkformat**](#function-eformat_to_vkformat) (EFormat format) <br> |
|  vk::FrontFace | [**efrontface\_to\_vkfrontface**](#function-efrontface_to_vkfrontface) (EFrontFace front\_face) <br> |
|  vk::PolygonMode | [**epolygonmode\_to\_vkpolygonmode**](#function-epolygonmode_to_vkpolygonmode) (EPolygonMode polygon\_mode) <br> |
|  vk::SamplerAddressMode | [**erepeatmode\_to\_vkrepeatmode**](#function-erepeatmode_to_vkrepeatmode) (ERepeatMode repeat\_mode) <br> |
|  vk::ShaderStageFlags | [**eshader\_to\_vkshader**](#function-eshader_to_vkshader) (EShader shader) <br> |
|  vk::PrimitiveTopology | [**etopology\_to\_vktopology**](#function-etopology_to_vktopology) (ETopology topology) <br> |
|  std::vector&lt; const char \* &gt; | [**get\_device\_extensions**](#function-get_device_extensions) () <br> |
|  bool | [**get\_extensions**](#function-get_extensions) (std::vector&lt; const char \* &gt; \* inst\_exts, std::vector&lt; const char \* &gt; \* dev\_exts) <br> |
|  bool | [**get\_instance\_extensions**](#function-get_instance_extensions) (std::vector&lt; const char \* &gt; \* inst\_exts) <br> |
|  void | [**get\_window\_size**](#function-get_window_size) (void \* native\_window, uint32\_t \* x, uint32\_t \* y) <br> |
|  void | [**load\_functions**](#function-load_functions) (vk::Instance instance, vk::Device device) <br> |
|  void | [**log\_error**](#function-log_error) (const std::string & message, const vkb::Error & error) <br> |
|  auto VKAPI\_PTR | [**vk\_alloc\_callback**](#function-vk_alloc_callback) (void \* pUserData, size\_t size, size\_t alignment, VkSystemAllocationScope allocationScope) <br> |
|  VKAPI\_ATTR VkBool32 VKAPI\_CALL | [**vk\_debug\_callback**](#function-vk_debug_callback) (VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT \* data, void \* user\_data) <br> |
|  auto VKAPI\_PTR | [**vk\_free\_callback**](#function-vk_free_callback) (void \* pUserData, void \* pMemory) <br> |
|  auto VKAPI\_PTR | [**vk\_realloc\_callback**](#function-vk_realloc_callback) (void \* pUserData, void \* pOriginal, size\_t size, size\_t alignment, VkSystemAllocationScope allocationScope) <br> |
|  typename std::remove\_cvref\_t&lt; T &gt;::NativeType \* | [**vkcast**](#function-vkcast) (T & obj) <br>_Helper function for converting vk:: namespaced types from vulkan.hpp to their Vulkan C counterparts._  |
|  typename std::remove\_cvref\_t&lt; T &gt;::NativeType \* | [**vkcast**](#function-vkcast) (T \* obj) <br>_Helper function for converting vk:: namespaced types from vulkan.hpp to their Vulkan C counterparts._  |
|  auto | [**vkconvert**](#function-vkconvert) (T value) <br> |




























## Public Static Attributes Documentation




### variable MAX\_FRAMES\_IN\_FLIGHT 

```C++
size_t aby::rhi::vulkan::MAX_FRAMES_IN_FLIGHT;
```




<hr>
## Public Functions Documentation




### function allocator 

```C++
VkAllocationCallbacks * aby::rhi::vulkan::allocator () 
```




<hr>



### function create\_surface 

```C++
bool aby::rhi::vulkan::create_surface (
    void * native_window,
    VkInstance instance,
    VkSurfaceKHR * surface
) 
```




<hr>



### function eblendfactor\_to\_vkblendfactor 

```C++
vk::BlendFactor aby::rhi::vulkan::eblendfactor_to_vkblendfactor (
    EBlendFactor blend_factor
) 
```




<hr>



### function eblendop\_to\_vkblendop 

```C++
vk::BlendOp aby::rhi::vulkan::eblendop_to_vkblendop (
    EBlendOp blend_op
) 
```




<hr>



### function ecompareop\_to\_vkcompareop 

```C++
vk::CompareOp aby::rhi::vulkan::ecompareop_to_vkcompareop (
    ECompareOp compare_op
) 
```




<hr>



### function ecullmode\_to\_vkcullmode 

```C++
vk::CullModeFlags aby::rhi::vulkan::ecullmode_to_vkcullmode (
    ECullMode cull_mode
) 
```




<hr>



### function efiltering\_to\_vkfilter 

```C++
std::pair< vk::Filter, vk::SamplerMipmapMode > aby::rhi::vulkan::efiltering_to_vkfilter (
    EFiltering filtering
) 
```




<hr>



### function eformat\_to\_vkformat 

```C++
vk::Format aby::rhi::vulkan::eformat_to_vkformat (
    EFormat format
) 
```




<hr>



### function efrontface\_to\_vkfrontface 

```C++
vk::FrontFace aby::rhi::vulkan::efrontface_to_vkfrontface (
    EFrontFace front_face
) 
```




<hr>



### function epolygonmode\_to\_vkpolygonmode 

```C++
vk::PolygonMode aby::rhi::vulkan::epolygonmode_to_vkpolygonmode (
    EPolygonMode polygon_mode
) 
```




<hr>



### function erepeatmode\_to\_vkrepeatmode 

```C++
vk::SamplerAddressMode aby::rhi::vulkan::erepeatmode_to_vkrepeatmode (
    ERepeatMode repeat_mode
) 
```




<hr>



### function eshader\_to\_vkshader 

```C++
vk::ShaderStageFlags aby::rhi::vulkan::eshader_to_vkshader (
    EShader shader
) 
```




<hr>



### function etopology\_to\_vktopology 

```C++
vk::PrimitiveTopology aby::rhi::vulkan::etopology_to_vktopology (
    ETopology topology
) 
```




<hr>



### function get\_device\_extensions 

```C++
std::vector< const char * > aby::rhi::vulkan::get_device_extensions () 
```




<hr>



### function get\_extensions 

```C++
bool aby::rhi::vulkan::get_extensions (
    std::vector< const char * > * inst_exts,
    std::vector< const char * > * dev_exts
) 
```




<hr>



### function get\_instance\_extensions 

```C++
bool aby::rhi::vulkan::get_instance_extensions (
    std::vector< const char * > * inst_exts
) 
```




<hr>



### function get\_window\_size 

```C++
void aby::rhi::vulkan::get_window_size (
    void * native_window,
    uint32_t * x,
    uint32_t * y
) 
```




<hr>



### function load\_functions 

```C++
void aby::rhi::vulkan::load_functions (
    vk::Instance instance,
    vk::Device device
) 
```




<hr>



### function log\_error 

```C++
void aby::rhi::vulkan::log_error (
    const std::string & message,
    const vkb::Error & error
) 
```




<hr>



### function vk\_alloc\_callback 

```C++
auto VKAPI_PTR aby::rhi::vulkan::vk_alloc_callback (
    void * pUserData,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope allocationScope
) 
```




<hr>



### function vk\_debug\_callback 

```C++
VKAPI_ATTR VkBool32 VKAPI_CALL aby::rhi::vulkan::vk_debug_callback (
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT * data,
    void * user_data
) 
```




<hr>



### function vk\_free\_callback 

```C++
auto VKAPI_PTR aby::rhi::vulkan::vk_free_callback (
    void * pUserData,
    void * pMemory
) 
```




<hr>



### function vk\_realloc\_callback 

```C++
auto VKAPI_PTR aby::rhi::vulkan::vk_realloc_callback (
    void * pUserData,
    void * pOriginal,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope allocationScope
) 
```




<hr>



### function vkcast 

_Helper function for converting vk:: namespaced types from vulkan.hpp to their Vulkan C counterparts._ 
```C++
template<typename T>
typename std::remove_cvref_t< T >::NativeType * aby::rhi::vulkan::vkcast (
    T & obj
) 
```





**Parameters:**


* `obj` reference to a vk:: namespaced object 



**Returns:**

Vulkan-C pointee 





        

<hr>



### function vkcast 

_Helper function for converting vk:: namespaced types from vulkan.hpp to their Vulkan C counterparts._ 
```C++
template<typename T>
typename std::remove_cvref_t< T >::NativeType * aby::rhi::vulkan::vkcast (
    T * obj
) 
```





**Parameters:**


* `obj` pointer to a vk:: namespaced object 



**Returns:**

Vulkan-C pointee 





        

<hr>



### function vkconvert 

```C++
template<typename T>
auto aby::rhi::vulkan::vkconvert (
    T value
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-buffer.hpp`

