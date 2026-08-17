

# Class aby::rhi::vulkan::RenderPassBuilder



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md)








Inherits the following classes: [aby::rhi::RenderPassBuilder](classaby_1_1rhi_1_1_render_pass_builder.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RenderPassBuilder**](#function-renderpassbuilder) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**add\_color\_attachment**](#function-add_color_attachment) ([**Resource**](classaby_1_1rhi_1_1_resource.md) texture, bool is\_present\_target=false) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**add\_shader**](#function-add_shader-12) (const fs::path & rel\_path) <br>_The render pass will own the created shader._  |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**add\_shader**](#function-add_shader-22) ([**Resource**](classaby_1_1rhi_1_1_resource.md) shader) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**add\_uniform**](#function-add_uniform) (std::string\_view name, uint32\_t binding, EShader stage) <br> |
| virtual std::shared\_ptr&lt; [**rhi::RenderPass**](classaby_1_1rhi_1_1_render_pass.md) &gt; | [**build**](#function-build) () override<br> |
| virtual void | [**clear**](#function-clear) () override<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**disable\_blending**](#function-disable_blending) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**disable\_depthtest**](#function-disable_depthtest) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_antialiasing**](#function-set_antialiasing) (EAntiAliasing aliasing) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_blend\_alpha**](#function-set_blend_alpha) ([**Blend**](structaby_1_1rhi_1_1_blend.md) blend, size\_t attachment=0) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_blend\_color**](#function-set_blend_color) (bool enable, [**Blend**](structaby_1_1rhi_1_1_blend.md) blend, size\_t attachment=0) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_blend\_mask**](#function-set_blend_mask) (EChannels mask, size\_t attachment=0) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_cull\_mode**](#function-set_cull_mode) (ECullMode mode, EFrontFace front\_face) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_depth**](#function-set_depth) (bool enable\_test, bool enable\_write, ECompareOp compare\_op) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_depth\_format**](#function-set_depth_format) (EFormat format) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_polygon\_mode**](#function-set_polygon_mode) (EPolygonMode mode, float line\_width) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_stencil**](#function-set_stencil) (bool enable, ECompareOp compare\_op) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**set\_topology**](#function-set_topology) (ETopology topology) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1vulkan_1_1_render_pass_builder.md) &override | [**use\_default\_attachment\_formats**](#function-use_default_attachment_formats) () <br>[_**Renderer**_](classaby_1_1rhi_1_1vulkan_1_1_renderer.md) _determinant._ |


## Public Functions inherited from aby::rhi::RenderPassBuilder

See [aby::rhi::RenderPassBuilder](classaby_1_1rhi_1_1_render_pass_builder.md)

| Type | Name |
| ---: | :--- |
|   | [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md#function-renderpassbuilder) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_color\_attachment**](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_color_attachment) ([**Resource**](classaby_1_1rhi_1_1_resource.md) texture, bool is\_present\_target=false) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_shader**](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_shader-12) (const fs::path & rel\_path) = 0<br>_The render pass will own the created shader._  |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_shader**](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_shader-22) ([**Resource**](classaby_1_1rhi_1_1_resource.md) shader) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**add\_uniform**](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_uniform) (std::string\_view name, uint32\_t binding, EShader stage) = 0<br> |
| virtual std::shared\_ptr&lt; [**RenderPass**](classaby_1_1rhi_1_1_render_pass.md) &gt; | [**build**](classaby_1_1rhi_1_1_render_pass_builder.md#function-build) () = 0<br> |
| virtual void | [**clear**](classaby_1_1rhi_1_1_render_pass_builder.md#function-clear) () = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**disable\_blending**](classaby_1_1rhi_1_1_render_pass_builder.md#function-disable_blending) () = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**disable\_depthtest**](classaby_1_1rhi_1_1_render_pass_builder.md#function-disable_depthtest) () = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_antialiasing**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_antialiasing) (EAntiAliasing aliasing) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_alpha**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_alpha-12) ([**Blend**](structaby_1_1rhi_1_1_blend.md) blend, size\_t attachment=0) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_alpha**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_alpha-22) ([**Blend**](structaby_1_1rhi_1_1_blend.md) blend, std::set&lt; size\_t &gt; attachments) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_color**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_color-12) (bool enable, [**Blend**](structaby_1_1rhi_1_1_blend.md) blend, size\_t attachment=0) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_color**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_color-22) (bool enable, [**Blend**](structaby_1_1rhi_1_1_blend.md) blend, std::set&lt; size\_t &gt; attachments) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_mask**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_mask-12) (EChannels mask, size\_t attachment=0) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_blend\_mask**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_mask-22) (EChannels mask, std::set&lt; size\_t &gt; attachments) <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_cull\_mode**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_cull_mode) (ECullMode mode, EFrontFace front\_face) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_depth**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_depth) (bool enable\_test, bool enable\_write, ECompareOp compare\_op) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_depth\_format**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_depth_format) (EFormat format) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_polygon\_mode**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_polygon_mode) (EPolygonMode mode, float line\_width) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_stencil**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_stencil) (bool enable, ECompareOp compare\_op) = 0<br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**set\_topology**](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_topology) (ETopology topology) = 0<br> |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_all\_defaults**](classaby_1_1rhi_1_1_render_pass_builder.md#function-use_all_defaults) () <br> |
| virtual [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_attachment\_formats**](classaby_1_1rhi_1_1_render_pass_builder.md#function-use_default_attachment_formats) () = 0<br>[_**Renderer**_](classaby_1_1rhi_1_1_renderer.md) _determinant._ |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_cull\_mode**](classaby_1_1rhi_1_1_render_pass_builder.md#function-use_default_cull_mode) () <br>_ECullMode::none, EFrontFace::clockwise._  |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_polygon\_mode**](classaby_1_1rhi_1_1_render_pass_builder.md#function-use_default_polygon_mode) () <br>_EPolygonMode::fill._  |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) & | [**use\_default\_topology**](classaby_1_1rhi_1_1_render_pass_builder.md#function-use_default_topology) () <br>_ETopologoy::triangle\_list._  |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) & | [**vertex\_description\_builder**](classaby_1_1rhi_1_1_render_pass_builder.md#function-vertex_description_builder) () <br> |
| virtual  | [**~RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md#function-renderpassbuilder) () = default<br> |




## Public Static Functions inherited from aby::rhi::RenderPassBuilder

See [aby::rhi::RenderPassBuilder](classaby_1_1rhi_1_1_render_pass_builder.md)

| Type | Name |
| ---: | :--- |
|  std::unique\_ptr&lt; [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) &gt; | [**create**](classaby_1_1rhi_1_1_render_pass_builder.md#function-create) () <br> |












## Protected Attributes inherited from aby::rhi::RenderPassBuilder

See [aby::rhi::RenderPassBuilder](classaby_1_1rhi_1_1_render_pass_builder.md)

| Type | Name |
| ---: | :--- |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) | [**m\_VIDB**](classaby_1_1rhi_1_1_render_pass_builder.md#variable-m_vidb)  <br> |






































## Public Functions Documentation




### function RenderPassBuilder 

```C++
aby::rhi::vulkan::RenderPassBuilder::RenderPassBuilder () 
```




<hr>



### function add\_color\_attachment 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::add_color_attachment (
    Resource texture,
    bool is_present_target=false
) 
```



Implements [*aby::rhi::RenderPassBuilder::add\_color\_attachment*](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_color_attachment)


<hr>



### function add\_shader [1/2]

_The render pass will own the created shader._ 
```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::add_shader (
    const fs::path & rel_path
) 
```



Implements [*aby::rhi::RenderPassBuilder::add\_shader*](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_shader-12)


<hr>



### function add\_shader [2/2]

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::add_shader (
    Resource shader
) 
```



Implements [*aby::rhi::RenderPassBuilder::add\_shader*](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_shader-22)


<hr>



### function add\_uniform 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::add_uniform (
    std::string_view name,
    uint32_t binding,
    EShader stage
) 
```



Implements [*aby::rhi::RenderPassBuilder::add\_uniform*](classaby_1_1rhi_1_1_render_pass_builder.md#function-add_uniform)


<hr>



### function build 

```C++
virtual std::shared_ptr< rhi::RenderPass > aby::rhi::vulkan::RenderPassBuilder::build () override
```



Implements [*aby::rhi::RenderPassBuilder::build*](classaby_1_1rhi_1_1_render_pass_builder.md#function-build)


<hr>



### function clear 

```C++
virtual void aby::rhi::vulkan::RenderPassBuilder::clear () override
```



Implements [*aby::rhi::RenderPassBuilder::clear*](classaby_1_1rhi_1_1_render_pass_builder.md#function-clear)


<hr>



### function disable\_blending 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::disable_blending () 
```



Implements [*aby::rhi::RenderPassBuilder::disable\_blending*](classaby_1_1rhi_1_1_render_pass_builder.md#function-disable_blending)


<hr>



### function disable\_depthtest 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::disable_depthtest () 
```



Implements [*aby::rhi::RenderPassBuilder::disable\_depthtest*](classaby_1_1rhi_1_1_render_pass_builder.md#function-disable_depthtest)


<hr>



### function set\_antialiasing 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_antialiasing (
    EAntiAliasing aliasing
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_antialiasing*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_antialiasing)


<hr>



### function set\_blend\_alpha 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_blend_alpha (
    Blend blend,
    size_t attachment=0
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_blend\_alpha*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_alpha-12)


<hr>



### function set\_blend\_color 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_blend_color (
    bool enable,
    Blend blend,
    size_t attachment=0
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_blend\_color*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_color-12)


<hr>



### function set\_blend\_mask 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_blend_mask (
    EChannels mask,
    size_t attachment=0
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_blend\_mask*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_blend_mask-12)


<hr>



### function set\_cull\_mode 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_cull_mode (
    ECullMode mode,
    EFrontFace front_face
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_cull\_mode*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_cull_mode)


<hr>



### function set\_depth 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_depth (
    bool enable_test,
    bool enable_write,
    ECompareOp compare_op
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_depth*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_depth)


<hr>



### function set\_depth\_format 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_depth_format (
    EFormat format
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_depth\_format*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_depth_format)


<hr>



### function set\_polygon\_mode 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_polygon_mode (
    EPolygonMode mode,
    float line_width
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_polygon\_mode*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_polygon_mode)


<hr>



### function set\_stencil 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_stencil (
    bool enable,
    ECompareOp compare_op
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_stencil*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_stencil)


<hr>



### function set\_topology 

```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::set_topology (
    ETopology topology
) 
```



Implements [*aby::rhi::RenderPassBuilder::set\_topology*](classaby_1_1rhi_1_1_render_pass_builder.md#function-set_topology)


<hr>



### function use\_default\_attachment\_formats 

[_**Renderer**_](classaby_1_1rhi_1_1vulkan_1_1_renderer.md) _determinant._
```C++
virtual RenderPassBuilder &override aby::rhi::vulkan::RenderPassBuilder::use_default_attachment_formats () 
```



Implements [*aby::rhi::RenderPassBuilder::use\_default\_attachment\_formats*](classaby_1_1rhi_1_1_render_pass_builder.md#function-use_default_attachment_formats)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-render-pass.hpp`

