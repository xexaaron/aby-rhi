

# Namespace aby::rhi



[**Namespace List**](namespaces.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md)



_Operators for enum classes._ [More...](#detailed-description)














## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**detail**](namespaceaby_1_1rhi_1_1detail.md) <br> |
| namespace | [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Blend**](structaby_1_1rhi_1_1_blend.md) <br> |
| class | [**Buffer**](classaby_1_1rhi_1_1_buffer.md) <br> |
| class | [**Context**](classaby_1_1rhi_1_1_context.md) <br> |
| struct | [**ContextParams**](structaby_1_1rhi_1_1_context_params.md) <br> |
| class | [**DefaultAllocator**](classaby_1_1rhi_1_1_default_allocator.md) <br> |
| class | [**DefaultFileIO**](classaby_1_1rhi_1_1_default_file_i_o.md) <br> |
| struct | [**DefaultJobQueue**](structaby_1_1rhi_1_1_default_job_queue.md) <br> |
| class | [**DefaultJobSystem**](classaby_1_1rhi_1_1_default_job_system.md) <br> |
| class | [**DefaultLogger**](classaby_1_1rhi_1_1_default_logger.md) <br> |
| class | [**DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md) <br>_Draw Cmd ares to be submitted to a created render pass each frame._  |
| struct | [**GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) <br> |
| class | [**IAllocator**](classaby_1_1rhi_1_1_i_allocator.md) <br>[_**IAllocator**_](classaby_1_1rhi_1_1_i_allocator.md) _interface for alloc/free/realloc._ |
| class | [**IFileIO**](classaby_1_1rhi_1_1_i_file_i_o.md) <br>[_**IFileIO**_](classaby_1_1rhi_1_1_i_file_i_o.md) _interface for reading/writing data. Used for reading/writing to/from shaders/textures._ |
| class | [**IInterface**](classaby_1_1rhi_1_1_i_interface.md) <br>_Base class for interfaces providing a common subset of debugging information._  |
| class | [**IJobSystem**](classaby_1_1rhi_1_1_i_job_system.md) <br> |
| class | [**ILogger**](classaby_1_1rhi_1_1_i_logger.md) <br>[_**ILogger**_](classaby_1_1rhi_1_1_i_logger.md) _message for passing pre-formatted messages to the application._ |
| class | [**IndexBuffer**](classaby_1_1rhi_1_1_index_buffer.md) <br> |
| class | [**RenderPass**](classaby_1_1rhi_1_1_render_pass.md) <br>_Use the_ [_**RenderPassBuilder**_](classaby_1_1rhi_1_1_render_pass_builder.md) _to construct this._ |
| class | [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) <br> |
| class | [**Renderer**](classaby_1_1rhi_1_1_renderer.md) <br> |
| class | [**Resource**](classaby_1_1rhi_1_1_resource.md) <br>_A resource is a handle to the resource data contained with the_ [_**Context**_](classaby_1_1rhi_1_1_context.md) _. This is used for loading data asynchronously while still being able to give out valid usable IDs._ |
| class | [**ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md) &lt;typename T, ResourceType&gt;<br>_A resource container that owns the resources and performs cleanup._  |
| class | [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md) &lt;typename T, ResourceType&gt;<br>[_**ResourcePtr**_](classaby_1_1rhi_1_1_resource_ptr.md) _class that abstracts the synchronization mechanisms from the user._ |
| class | [**Shader**](classaby_1_1rhi_1_1_shader.md) <br> |
| class | [**Texture**](classaby_1_1rhi_1_1_texture.md) <br> |
| struct | [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) <br> |
| struct | [**UniformBufferDesc**](structaby_1_1rhi_1_1_uniform_buffer_desc.md) <br> |
| struct | [**VertexAttributeDesc**](structaby_1_1rhi_1_1_vertex_attribute_desc.md) <br> |
| class | [**VertexBuffer**](classaby_1_1rhi_1_1_vertex_buffer.md) <br> |
| struct | [**VertexInput**](structaby_1_1rhi_1_1_vertex_input.md) <br> |
| class | [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) <br> |
| struct | [**VertexShaderDesc**](structaby_1_1rhi_1_1_vertex_shader_desc.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**EAllocation**](#enum-eallocation)  <br> |
| enum  | [**EAntiAliasing**](#enum-eantialiasing)  <br> |
| enum  | [**EBlendFactor**](#enum-eblendfactor)  <br> |
| enum  | [**EBlendOp**](#enum-eblendop)  <br> |
| enum uint8\_t | [**EChannels**](#enum-echannels)  <br> |
| enum  | [**ECompareOp**](#enum-ecompareop)  <br> |
| enum  | [**ECullMode**](#enum-ecullmode)  <br> |
| enum  | [**EFiltering**](#enum-efiltering)  <br> |
| enum  | [**EFormat**](#enum-eformat)  <br> |
| enum  | [**EFrontFace**](#enum-efrontface)  <br> |
| enum  | [**EJobPriority**](#enum-ejobpriority)  <br> |
| enum  | [**EJobQueue**](#enum-ejobqueue)  <br> |
| enum  | [**ELogLevel**](#enum-eloglevel)  <br> |
| enum  | [**EPolygonMode**](#enum-epolygonmode)  <br> |
| enum  | [**ERenderer**](#enum-erenderer)  <br> |
| enum  | [**ERepeatMode**](#enum-erepeatmode)  <br> |
| enum uint32\_t | [**EResource**](#enum-eresource)  <br> |
| enum uint16\_t | [**EResourceState**](#enum-eresourcestate)  <br> |
| enum  | [**EShader**](#enum-eshader)  <br> |
| enum  | [**ETextureUsage**](#enum-etextureusage)  <br> |
| enum  | [**ETopology**](#enum-etopology)  <br> |
| enum  | [**EWindow**](#enum-ewindow)  <br> |
| enum uint32\_t | [**ResourceID**](#enum-resourceid)  <br> |
| typedef [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; [**Shader**](classaby_1_1rhi_1_1_shader.md), EResource::shader &gt; | [**ShaderPtr**](#typedef-shaderptr)  <br> |
| typedef [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; [**Texture**](classaby_1_1rhi_1_1_texture.md), EResource::texture &gt; | [**TexturePtr**](#typedef-textureptr)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ABY\_RHI\_PACKED**](#function-aby_rhi_packed) (struct Color { Color(float scalar=0.f);Color(float r, float g, float b, float a=1.f);union { float rgba[4];struct { float r;float g;float b;float a;};};}) <br> |
|   | [**ABY\_RHI\_PACKED**](#function-aby_rhi_packed) (struct vec\_tag{}) <br> |
|   | [**ABY\_RHI\_PACKED**](#function-aby_rhi_packed) (template&lt; typename T &gt; struct vec2 :vec\_tag { using underlying\_type=T;template&lt; typename U &gt; requires(requires(const U &v) { v.x;v.y;} &&!std::derived\_from&lt; std::remove\_cvref\_t&lt; U &gt;, vec\_tag &gt;) vec2(const U &vec\_type);vec2(T x, T y);vec2(T s);T x;T y;}) <br> |
|   | [**ABY\_RHI\_PACKED**](#function-aby_rhi_packed) (template&lt; typename T &gt; struct vec3 :vec\_tag { using underlying\_type=T;template&lt; typename U &gt; requires(requires(const U &v) { v.x;v.y;v.z;} &&!std::derived\_from&lt; std::remove\_cvref\_t&lt; U &gt;, vec\_tag &gt;) vec3(const U &vec\_type);vec3(T x, T y, T z);vec3(T s);T x;T y;T z;}) <br> |
|   | [**ABY\_RHI\_PACKED**](#function-aby_rhi_packed) (template&lt; typename T &gt; struct vec4 :vec\_tag { using underlying\_type=T;template&lt; typename U &gt; requires(requires(const U &v) { v.x;v.y;v.z;v.w;} &&!std::derived\_from&lt; std::remove\_cvref\_t&lt; U &gt;, vec\_tag &gt;) vec4(const U &vec\_type);vec4(T x, T y, T z, T w);vec4(T s);T x;T y;T z;T w;}) <br> |
|   | [**ABY\_RHI\_PACKED**](#function-aby_rhi_packed) (struct mat4 { template&lt; typename T &gt; mat4(const T &mat\_type) { std::memcpy(data, reinterpret\_cast&lt; float \* &gt;(\*mat\_type), sizeof(float) \*16);} float data[16];}) <br> |
|  [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; T, ResourceType &gt; | [**create\_resource**](#function-create_resource) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource, [**ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md)&lt; T, ResourceType &gt; & container) <br> |
|  EChannels | [**operator&**](#function-operator) (EChannels lhs, EChannels rhs) <br> |
|  aby::rhi::EChannels | [**operator\|**](#function-operator_1) (EChannels lhs, EChannels rhs) <br> |




























## Detailed Description


A set of interfaces that can be set by the user to modify parts of the library.




**Parameters:**


* [**IInterface**](classaby_1_1rhi_1_1_i_interface.md) Do not use directly. it is the base class of interfaces. 
* [**ILogger**](classaby_1_1rhi_1_1_i_logger.md) Logging interface to pass pre-formatted messages to the application 
* [**IAllocator**](classaby_1_1rhi_1_1_i_allocator.md) Allocator interface used in the backed and for other allocations. 




    
## Public Types Documentation




### enum EAllocation 

```C++
enum aby::rhi::EAllocation {
    backend
};
```




<hr>



### enum EAntiAliasing 

```C++
enum aby::rhi::EAntiAliasing {
    none,
    msaa2x,
    msaa4x,
    msaa8x
};
```




<hr>



### enum EBlendFactor 

```C++
enum aby::rhi::EBlendFactor {
    zero,
    one,
    src_color,
    one_minus_src_color,
    dst_color,
    one_minus_dst_color,
    src_alpha,
    one_minus_src_alpha,
    dst_alpha,
    one_minus_dst_alpha,
    constant_color,
    one_minus_constant_color,
    constant_alpha,
    one_minus_constant_alpha,
    src_alpha_saturate,
    src_one_color,
    one_minus_src_one_color,
    src_one_alpha,
    one_minus_src_one_alpha
};
```




<hr>



### enum EBlendOp 

```C++
enum aby::rhi::EBlendOp {
    add,
    sub,
    reverse_sub,
    min,
    max
};
```




<hr>



### enum EChannels 

```C++
enum aby::rhi::EChannels {
    none = 0,
    r = 1 << 0,
    g = 1 << 1,
    b = 1 << 2,
    a = 1 << 3,
    rg = r | g,
    rb = r | b,
    ra = r | a,
    gb = g | b,
    ga = g | a,
    ba = b | a,
    rgb = r | g | b,
    rga = r | g | a,
    rba = r | b | a,
    gba = g | b | a,
    rgba = r | g | b | a
};
```




<hr>



### enum ECompareOp 

```C++
enum aby::rhi::ECompareOp {
    never,
    less,
    eq,
    less_eq,
    greater,
    neq,
    greater_eq,
    always
};
```




<hr>



### enum ECullMode 

```C++
enum aby::rhi::ECullMode {
    none,
    front,
    back,
    front_and_back
};
```




<hr>



### enum EFiltering 

```C++
enum aby::rhi::EFiltering {
    linear,
    nearest,
    cubic
};
```




<hr>



### enum EFormat 

```C++
enum aby::rhi::EFormat {
    none,
    rgba_f32,
    rgb_f32,
    rg_f32,
    r_f32,
    rgba_f16,
    rgb_f16,
    rg_f16,
    r_f16,
    rgba_i32,
    rgb_i32,
    rg_i32,
    r_i32,
    rgba_u32,
    rgb_u32,
    rg_u32,
    r_u32
};
```




<hr>



### enum EFrontFace 

```C++
enum aby::rhi::EFrontFace {
    clockwise,
    counter_clockwise
};
```




<hr>



### enum EJobPriority 

```C++
enum aby::rhi::EJobPriority {
    low = 0,
    medium = 1,
    high = 2,
    critical = 3
};
```




<hr>



### enum EJobQueue 

```C++
enum aby::rhi::EJobQueue {
    textures = 0,
    shaders = 1,
    caching = 2,
    max_queues
};
```




<hr>



### enum ELogLevel 

```C++
enum aby::rhi::ELogLevel {
    debug,
    trace,
    info,
    warn,
    error,
    fatal
};
```




<hr>



### enum EPolygonMode 

```C++
enum aby::rhi::EPolygonMode {
    fill,
    line,
    point
};
```




<hr>



### enum ERenderer 

```C++
enum aby::rhi::ERenderer {
    vulkan
};
```




<hr>



### enum ERepeatMode 

```C++
enum aby::rhi::ERepeatMode {
    repeat,
    mirrored,
    clamp_to_edge,
    clamp_to_border,
    mirrored_clamp_to_edge
};
```




<hr>



### enum EResource 

```C++
enum aby::rhi::EResource {
    none,
    texture,
    shader
};
```




<hr>



### enum EResourceState 

```C++
enum aby::rhi::EResourceState {
    invalid,
    loading,
    loaded,
    failed
};
```




<hr>



### enum EShader 

```C++
enum aby::rhi::EShader {
    none = 0,
    vert = 1,
    frag = 2,
    px = frag,
    comp = 3,
    geom = 4
};
```




<hr>



### enum ETextureUsage 

```C++
enum aby::rhi::ETextureUsage {
    albedo,
    material
};
```




<hr>



### enum ETopology 

```C++
enum aby::rhi::ETopology {
    point_list,
    line_list,
    line_strip,
    triangle_list,
    triangle_strip,
    triangle_fan
};
```




<hr>



### enum EWindow 

```C++
enum aby::rhi::EWindow {
    win32,
    x11,
    xcb,
    wayland,
    metal
};
```




<hr>



### enum ResourceID 

```C++
enum aby::rhi::ResourceID {
    invalid = UINT32_MAX
};
```




<hr>



### typedef ShaderPtr 

```C++
using aby::rhi::ShaderPtr =  ResourcePtr<Shader, EResource::shader>;
```




<hr>



### typedef TexturePtr 

```C++
using aby::rhi::TexturePtr =  ResourcePtr<Texture, EResource::texture>;
```




<hr>
## Public Functions Documentation




### function ABY\_RHI\_PACKED 

```C++
aby::rhi::ABY_RHI_PACKED (
    struct Color { Color(float scalar=0.f);Color(float r, float g, float b, float a=1.f);union { float rgba[4];struct { float r;float g;float b;float a;};};}
) 
```




<hr>



### function ABY\_RHI\_PACKED 

```C++
aby::rhi::ABY_RHI_PACKED (
    struct vec_tag{}
) 
```




<hr>



### function ABY\_RHI\_PACKED 

```C++
aby::rhi::ABY_RHI_PACKED (
    template< typename T > struct vec2 :vec_tag { using underlying_type=T;template< typename U > requires(requires(const U &v) { v.x;v.y;} &&!std::derived_from< std::remove_cvref_t< U >, vec_tag >) vec2(const U &vec_type);vec2(T x, T y);vec2(T s);T x;T y;}
) 
```




<hr>



### function ABY\_RHI\_PACKED 

```C++
aby::rhi::ABY_RHI_PACKED (
    template< typename T > struct vec3 :vec_tag { using underlying_type=T;template< typename U > requires(requires(const U &v) { v.x;v.y;v.z;} &&!std::derived_from< std::remove_cvref_t< U >, vec_tag >) vec3(const U &vec_type);vec3(T x, T y, T z);vec3(T s);T x;T y;T z;}
) 
```




<hr>



### function ABY\_RHI\_PACKED 

```C++
aby::rhi::ABY_RHI_PACKED (
    template< typename T > struct vec4 :vec_tag { using underlying_type=T;template< typename U > requires(requires(const U &v) { v.x;v.y;v.z;v.w;} &&!std::derived_from< std::remove_cvref_t< U >, vec_tag >) vec4(const U &vec_type);vec4(T x, T y, T z, T w);vec4(T s);T x;T y;T z;T w;}
) 
```




<hr>



### function ABY\_RHI\_PACKED 

```C++
aby::rhi::ABY_RHI_PACKED (
    struct mat4 { template< typename T > mat4(const T &mat_type) { std::memcpy(data, reinterpret_cast< float * >(*mat_type), sizeof(float) *16);} float data[16];}
) 
```




<hr>



### function create\_resource 

```C++
template<typename T, EResource ResourceType>
ResourcePtr < T, ResourceType > aby::rhi::create_resource (
    Resource resource,
    ResourceContainer < T, ResourceType > & container
) 
```




<hr>



### function operator& 

```C++
EChannels aby::rhi::operator& (
    EChannels lhs,
    EChannels rhs
) 
```




<hr>



### function operator\| 

```C++
aby::rhi::EChannels aby::rhi::operator| (
    EChannels lhs,
    EChannels rhs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-buffer.hpp`

