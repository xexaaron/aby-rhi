

# Class aby::rhi::DrawCmd



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**DrawCmd**](classaby_1_1rhi_1_1_draw_cmd.md)



_Draw Cmd ares to be submitted to a created render pass each frame._ 

* `#include <draw-cmd.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DrawCmd**](#function-drawcmd) (std::shared\_ptr&lt; [**VertexBuffer**](classaby_1_1rhi_1_1_vertex_buffer.md) &gt; vertices, std::shared\_ptr&lt; [**IndexBuffer**](classaby_1_1rhi_1_1_index_buffer.md) &gt; indices, uint32\_t instances=1) <br>[_**DrawCmd**_](classaby_1_1rhi_1_1_draw_cmd.md) _constructor._ |
|  [**IndexBuffer**](classaby_1_1rhi_1_1_index_buffer.md) \* | [**ibuff**](#function-ibuff) () const<br> |
|  uint32\_t | [**instances**](#function-instances) () const<br> |
|  void | [**set\_instance\_count**](#function-set_instance_count) (uint32\_t instance\_count) <br>_Set the instance count._  |
|  [**VertexBuffer**](classaby_1_1rhi_1_1_vertex_buffer.md) \* | [**vbuff**](#function-vbuff) () const<br> |




























## Public Functions Documentation




### function DrawCmd 

[_**DrawCmd**_](classaby_1_1rhi_1_1_draw_cmd.md) _constructor._
```C++
aby::rhi::DrawCmd::DrawCmd (
    std::shared_ptr< VertexBuffer > vertices,
    std::shared_ptr< IndexBuffer > indices,
    uint32_t instances=1
) 
```





**Parameters:**


* `vertices` A vertex buffer 
* `indices` An index buffer 
* `instances` The amount of times the same set of vertices and indices is to be drawn 




        

<hr>



### function ibuff 

```C++
IndexBuffer * aby::rhi::DrawCmd::ibuff () const
```




<hr>



### function instances 

```C++
uint32_t aby::rhi::DrawCmd::instances () const
```




<hr>



### function set\_instance\_count 

_Set the instance count._ 
```C++
void aby::rhi::DrawCmd::set_instance_count (
    uint32_t instance_count
) 
```





**Parameters:**


* `instance_count` The amount of times the same set of vertices and indices is to be drawn 




        

<hr>



### function vbuff 

```C++
VertexBuffer * aby::rhi::DrawCmd::vbuff () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/draw-cmd.hpp`

