

# Class aby::rhi::RenderPassBuilder



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md)










Inherited by the following classes: [aby::rhi::vulkan::RenderPassBuilder](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RenderPassBuilder**](#function-renderpassbuilder) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_color\_attachment**](#function-add_color_attachment) ([**Resource**](classaby_1_1rhi_1_1_resource.md) texture, bool is\_present\_target=false) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_shader**](#function-add_shader-12) (const fs::path & rel\_path) = 0<br>_The render pass will own the created shader._  |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_shader**](#function-add_shader-22) ([**Resource**](classaby_1_1rhi_1_1_resource.md) shader) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_uniform**](#function-add_uniform) (std::string\_view name, uint32\_t binding, EShader stage) = 0<br> |
| virtual std::shared\_ptr&lt; [**RenderPass**](classaby_1_1rhi_1_1_render_pass.md) &gt; | [**build**](#function-build) () = 0<br> |
| virtual void | [**clear**](#function-clear) () = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**disable\_blending**](#function-disable_blending) () = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**disable\_depthtest**](#function-disable_depthtest) () = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_antialiasing**](#function-set_antialiasing) (EAntiAliasing aliasing) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_alpha**](#function-set_blend_alpha-12) ([**Blend**](structaby_1_1rhi_1_1_blend.md) blend, size\_t attachment=0) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_alpha**](#function-set_blend_alpha-22) ([**Blend**](structaby_1_1rhi_1_1_blend.md) blend, std::set&lt; size\_t &gt; attachments) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_color**](#function-set_blend_color-12) (bool enable, [**Blend**](structaby_1_1rhi_1_1_blend.md) blend, size\_t attachment=0) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_color**](#function-set_blend_color-22) (bool enable, [**Blend**](structaby_1_1rhi_1_1_blend.md) blend, std::set&lt; size\_t &gt; attachments) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_mask**](#function-set_blend_mask-12) (EChannels mask, size\_t attachment=0) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_mask**](#function-set_blend_mask-22) (EChannels mask, std::set&lt; size\_t &gt; attachments) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_cull\_mode**](#function-set_cull_mode) (ECullMode mode, EFrontFace front\_face) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_depth**](#function-set_depth) (bool enable\_test, bool enable\_write, ECompareOp compare\_op) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_depth\_format**](#function-set_depth_format) (EFormat format) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_polygon\_mode**](#function-set_polygon_mode) (EPolygonMode mode, float line\_width) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_stencil**](#function-set_stencil) (bool enable, ECompareOp compare\_op) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_topology**](#function-set_topology) (ETopology topology) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_all\_defaults**](#function-use_all_defaults) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_attachment\_formats**](#function-use_default_attachment_formats) () = 0<br>[_**Renderer**_](classaby_1_1rhi_1_1_renderer.md) _determinant._ |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_cull\_mode**](#function-use_default_cull_mode) () <br>_ECullMode::none, EFrontFace::clockwise._  |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_polygon\_mode**](#function-use_default_polygon_mode) () <br>_EPolygonMode::fill._  |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_topology**](#function-use_default_topology) () <br>_ETopologoy::triangle\_list._  |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) & | [**vertex\_description\_builder**](#function-vertex_description_builder) () <br> |
| virtual  | [**~RenderPassBuilder**](#function-renderpassbuilder) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::unique\_ptr&lt; [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) &gt; | [**create**](#function-create) () <br> |






## Protected Attributes

| Type | Name |
| ---: | :--- |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) | [**m\_VIDB**](#variable-m_vidb)  <br> |




















## Public Functions Documentation




### function RenderPassBuilder 

```C++
aby::rhi::RenderPassBuilder::RenderPassBuilder () 
```




<hr>



### function add\_color\_attachment 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::add_color_attachment (
    Resource texture,
    bool is_present_target=false
) = 0
```




<hr>



### function add\_shader [1/2]

_The render pass will own the created shader._ 
```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::add_shader (
    const fs::path & rel_path
) = 0
```




<hr>



### function add\_shader [2/2]

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::add_shader (
    Resource shader
) = 0
```




<hr>



### function add\_uniform 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::add_uniform (
    std::string_view name,
    uint32_t binding,
    EShader stage
) = 0
```




<hr>



### function build 

```C++
virtual std::shared_ptr< RenderPass > aby::rhi::RenderPassBuilder::build () = 0
```




<hr>



### function clear 

```C++
virtual void aby::rhi::RenderPassBuilder::clear () = 0
```




<hr>



### function disable\_blending 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::disable_blending () = 0
```




<hr>



### function disable\_depthtest 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::disable_depthtest () = 0
```




<hr>



### function set\_antialiasing 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_antialiasing (
    EAntiAliasing aliasing
) = 0
```




<hr>



### function set\_blend\_alpha [1/2]

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_blend_alpha (
    Blend blend,
    size_t attachment=0
) = 0
```




<hr>



### function set\_blend\_alpha [2/2]

```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::set_blend_alpha (
    Blend blend,
    std::set< size_t > attachments
) 
```




<hr>



### function set\_blend\_color [1/2]

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_blend_color (
    bool enable,
    Blend blend,
    size_t attachment=0
) = 0
```




<hr>



### function set\_blend\_color [2/2]

```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::set_blend_color (
    bool enable,
    Blend blend,
    std::set< size_t > attachments
) 
```




<hr>



### function set\_blend\_mask [1/2]

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_blend_mask (
    EChannels mask,
    size_t attachment=0
) = 0
```




<hr>



### function set\_blend\_mask [2/2]

```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::set_blend_mask (
    EChannels mask,
    std::set< size_t > attachments
) 
```




<hr>



### function set\_cull\_mode 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_cull_mode (
    ECullMode mode,
    EFrontFace front_face
) = 0
```




<hr>



### function set\_depth 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_depth (
    bool enable_test,
    bool enable_write,
    ECompareOp compare_op
) = 0
```




<hr>



### function set\_depth\_format 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_depth_format (
    EFormat format
) = 0
```




<hr>



### function set\_polygon\_mode 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_polygon_mode (
    EPolygonMode mode,
    float line_width
) = 0
```




<hr>



### function set\_stencil 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_stencil (
    bool enable,
    ECompareOp compare_op
) = 0
```




<hr>



### function set\_topology 

```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::set_topology (
    ETopology topology
) = 0
```




<hr>



### function use\_all\_defaults 

```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::use_all_defaults () 
```




<hr>



### function use\_default\_attachment\_formats 

[_**Renderer**_](classaby_1_1rhi_1_1_renderer.md) _determinant._
```C++
virtual RenderPassBuilder & aby::rhi::RenderPassBuilder::use_default_attachment_formats () = 0
```




<hr>



### function use\_default\_cull\_mode 

_ECullMode::none, EFrontFace::clockwise._ 
```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::use_default_cull_mode () 
```




<hr>



### function use\_default\_polygon\_mode 

_EPolygonMode::fill._ 
```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::use_default_polygon_mode () 
```




<hr>



### function use\_default\_topology 

_ETopologoy::triangle\_list._ 
```C++
RenderPassBuilder & aby::rhi::RenderPassBuilder::use_default_topology () 
```




<hr>



### function vertex\_description\_builder 

```C++
VertexInputDescriptionBuilder & aby::rhi::RenderPassBuilder::vertex_description_builder () 
```




<hr>



### function ~RenderPassBuilder 

```C++
virtual aby::rhi::RenderPassBuilder::~RenderPassBuilder () = default
```




<hr>
## Public Static Functions Documentation




### function create 

```C++
static std::unique_ptr< RenderPassBuilder > aby::rhi::RenderPassBuilder::create () 
```




<hr>
## Protected Attributes Documentation




### variable m\_VIDB 

```C++
VertexInputDescriptionBuilder aby::rhi::RenderPassBuilder::m_VIDB;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/render-pass.hpp`

