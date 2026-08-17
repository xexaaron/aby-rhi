

# Class aby::rhi::VertexBuffer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**VertexBuffer**](classaby_1_1rhi_1_1_vertex_buffer.md)








Inherits the following classes: [aby::rhi::Buffer](classaby_1_1rhi_1_1_buffer.md)


Inherited by the following classes: [aby::rhi::vulkan::VertexBuffer](classaby_1_1rhi_1_1vulkan_1_1_vertex_buffer.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**VertexBuffer**](#function-vertexbuffer) (size\_t size, size\_t stride) <br>[_**VertexBuffer**_](classaby_1_1rhi_1_1_vertex_buffer.md) _constructor._ |
| virtual void | [**destroy**](#function-destroy) () = 0<br>_Cleanup all resources._  |
|  void | [**push**](#function-push) (void \* v) <br>_Push a vertex into the cpu buffer._  |
| virtual void | [**upload**](#function-upload) () = 0<br>_Upload the buffers cpu data to the GPU._  |


## Public Functions inherited from aby::rhi::Buffer

See [aby::rhi::Buffer](classaby_1_1rhi_1_1_buffer.md)

| Type | Name |
| ---: | :--- |
|   | [**Buffer**](classaby_1_1rhi_1_1_buffer.md#function-buffer) (size\_t size, size\_t stride) <br>[_**Buffer**_](classaby_1_1rhi_1_1_buffer.md) _constructor._ |
|  size\_t | [**capacity\_bytes**](classaby_1_1rhi_1_1_buffer.md#function-capacity_bytes) () const<br>_The total amount of bytes._  |
|  void | [**clear**](classaby_1_1rhi_1_1_buffer.md#function-clear) () <br>_Clears all data from the buffer (CPU ONLY)_  |
|  size\_t | [**count**](classaby_1_1rhi_1_1_buffer.md#function-count) () const<br>_The number of elements in the buffer._  |
| virtual void | [**destroy**](classaby_1_1rhi_1_1_buffer.md#function-destroy) () = 0<br>_Cleanup all resources._  |
|  size\_t | [**stride**](classaby_1_1rhi_1_1_buffer.md#function-stride) () const<br>_The byte size of each element._  |
| virtual void | [**upload**](classaby_1_1rhi_1_1_buffer.md#function-upload) () = 0<br>_Upload the buffers cpu data to the GPU._  |
|  size\_t | [**used\_bytes**](classaby_1_1rhi_1_1_buffer.md#function-used_bytes) () const<br>_The amount of bytes already pushed into the buffer._  |
| virtual  | [**~Buffer**](classaby_1_1rhi_1_1_buffer.md#function-buffer) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**VertexBuffer**](classaby_1_1rhi_1_1_vertex_buffer.md) &gt; | [**create**](#function-create) (size\_t size, size\_t stride) <br>_Create a vertex buffer._  |














## Protected Attributes inherited from aby::rhi::Buffer

See [aby::rhi::Buffer](classaby_1_1rhi_1_1_buffer.md)

| Type | Name |
| ---: | :--- |
|  size\_t | [**m\_Count**](classaby_1_1rhi_1_1_buffer.md#variable-m_count)  <br> |
|  uint8\_t \* | [**m\_Data**](classaby_1_1rhi_1_1_buffer.md#variable-m_data)  <br> |
|  size\_t | [**m\_Size**](classaby_1_1rhi_1_1_buffer.md#variable-m_size)  <br> |
|  size\_t | [**m\_Stride**](classaby_1_1rhi_1_1_buffer.md#variable-m_stride)  <br> |






































## Public Functions Documentation




### function VertexBuffer 

[_**VertexBuffer**_](classaby_1_1rhi_1_1_vertex_buffer.md) _constructor._
```C++
aby::rhi::VertexBuffer::VertexBuffer (
    size_t size,
    size_t stride
) 
```





**Parameters:**


* `size` The desired vertex count of the buffer 
* `stride` The byte size of each vertex in the buffer. 




        

<hr>



### function destroy 

_Cleanup all resources._ 
```C++
virtual void aby::rhi::VertexBuffer::destroy () = 0
```



Implements [*aby::rhi::Buffer::destroy*](classaby_1_1rhi_1_1_buffer.md#function-destroy)


<hr>



### function push 

_Push a vertex into the cpu buffer._ 
```C++
void aby::rhi::VertexBuffer::push (
    void * v
) 
```





**Parameters:**


* `v` A vertex pointer matching the stride set by the constructor. 




        

<hr>



### function upload 

_Upload the buffers cpu data to the GPU._ 
```C++
virtual void aby::rhi::VertexBuffer::upload () = 0
```



Implements [*aby::rhi::Buffer::upload*](classaby_1_1rhi_1_1_buffer.md#function-upload)


<hr>
## Public Static Functions Documentation




### function create 

_Create a vertex buffer._ 
```C++
static std::shared_ptr< VertexBuffer > aby::rhi::VertexBuffer::create (
    size_t size,
    size_t stride
) 
```





**Parameters:**


* `size` The desired vertex count of the buffer 
* `stride` The byte size of each vertex in the buffer. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/buffer.hpp`

