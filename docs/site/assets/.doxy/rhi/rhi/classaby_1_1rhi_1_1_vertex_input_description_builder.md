

# Class aby::rhi::VertexInputDescriptionBuilder



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**VertexInputDescriptionBuilder**](#function-vertexinputdescriptionbuilder) ([**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) \* rpb) <br> |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) & | [**add\_input**](#function-add_input-12) (size\_t bytes, EFormat format, size\_t offset) <br>_add a vertex input_  |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) & | [**add\_input**](#function-add_input-22) (EFormat format) <br>_Add a vertex shader input._  |
|  [**VertexInputDescriptionBuilder**](classaby_1_1rhi_1_1_vertex_input_description_builder.md) & | [**add\_inputs**](#function-add_inputs) (std::same\_as&lt; EFormat &gt; auto... formats) <br>_Add vertex shader inputs._  |
|  [**RenderPassBuilder**](classaby_1_1rhi_1_1_render_pass_builder.md) \* | [**build**](#function-build) () <br> |
|  std::vector&lt; [**VertexInput**](structaby_1_1rhi_1_1_vertex_input.md) &gt; & | [**inputs**](#function-inputs) () <br> |
|  size\_t | [**stride**](#function-stride) () <br> |




























## Public Functions Documentation




### function VertexInputDescriptionBuilder 

```C++
aby::rhi::VertexInputDescriptionBuilder::VertexInputDescriptionBuilder (
    RenderPassBuilder * rpb
) 
```




<hr>



### function add\_input [1/2]

_add a vertex input_ 
```C++
VertexInputDescriptionBuilder & aby::rhi::VertexInputDescriptionBuilder::add_input (
    size_t bytes,
    EFormat format,
    size_t offset
) 
```





**Parameters:**


* `bytes` The size of the vertex member. 
* `format` The format of the member (ie. vec2f -&gt; rg\_f32) 
* `offset` The offsetof the member compared to the Vertex structure. 




        

<hr>



### function add\_input [2/2]

_Add a vertex shader input._ 
```C++
template<auto Member>
inline VertexInputDescriptionBuilder & aby::rhi::VertexInputDescriptionBuilder::add_input (
    EFormat format
) 
```





**Template parameters:**


* `Member` in the format: &T::member 



**Parameters:**


* `format` The format corresponding to the members layout. (ie. vec2f -&gt; rg\_f32) 




        

<hr>



### function add\_inputs 

_Add vertex shader inputs._ 
```C++
template<auto... Member>
inline VertexInputDescriptionBuilder & aby::rhi::VertexInputDescriptionBuilder::add_inputs (
    std::same_as< EFormat > auto... formats
) 
```





**Template parameters:**


* `...Member` in the format: &T::member... 



**Parameters:**


* `formats` The format(s) corresponding to the members layout. (ie. vec2f -&gt; rg\_f32) 




        

<hr>



### function build 

```C++
RenderPassBuilder * aby::rhi::VertexInputDescriptionBuilder::build () 
```




<hr>



### function inputs 

```C++
std::vector< VertexInput > & aby::rhi::VertexInputDescriptionBuilder::inputs () 
```




<hr>



### function stride 

```C++
size_t aby::rhi::VertexInputDescriptionBuilder::stride () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/render-pass.hpp`

