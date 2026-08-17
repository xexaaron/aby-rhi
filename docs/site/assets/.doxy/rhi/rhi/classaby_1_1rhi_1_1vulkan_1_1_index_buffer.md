

# Class aby::rhi::vulkan::IndexBuffer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**IndexBuffer**](classaby_1_1rhi_1_1vulkan_1_1_index_buffer.md)








Inherits the following classes: [aby::rhi::IndexBuffer](classaby_1_1rhi_1_1_index_buffer.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IndexBuffer**](#function-indexbuffer) (size\_t size) <br> |
| virtual void | [**destroy**](#function-destroy) () override<br>_Cleanup all resources._  |
|  [**vulkan::Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) & | [**gpu**](#function-gpu) () <br> |
| virtual void | [**upload**](#function-upload) () override<br>_Upload the buffers cpu data to the GPU._  |


## Public Functions inherited from aby::rhi::IndexBuffer

See [aby::rhi::IndexBuffer](classaby_1_1rhi_1_1_index_buffer.md)

| Type | Name |
| ---: | :--- |
|   | [**IndexBuffer**](classaby_1_1rhi_1_1_index_buffer.md#function-indexbuffer) (size\_t size) <br>[_**IndexBuffer**_](classaby_1_1rhi_1_1_index_buffer.md) _constructor._ |
| virtual void | [**destroy**](classaby_1_1rhi_1_1_index_buffer.md#function-destroy) () = 0<br>_Cleanup all resources._  |
|  void | [**push**](classaby_1_1rhi_1_1_index_buffer.md#function-push) (uint32\_t index) <br>_Push an index into the cpu buffer._  |
| virtual void | [**upload**](classaby_1_1rhi_1_1_index_buffer.md#function-upload) () = 0<br>_Upload the buffers cpu data to the GPU._  |


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




## Public Static Functions inherited from aby::rhi::IndexBuffer

See [aby::rhi::IndexBuffer](classaby_1_1rhi_1_1_index_buffer.md)

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**IndexBuffer**](classaby_1_1rhi_1_1_index_buffer.md) &gt; | [**create**](classaby_1_1rhi_1_1_index_buffer.md#function-create) (size\_t size) <br>_Create an Index buffer._  |




















## Protected Attributes inherited from aby::rhi::Buffer

See [aby::rhi::Buffer](classaby_1_1rhi_1_1_buffer.md)

| Type | Name |
| ---: | :--- |
|  size\_t | [**m\_Count**](classaby_1_1rhi_1_1_buffer.md#variable-m_count)  <br> |
|  uint8\_t \* | [**m\_Data**](classaby_1_1rhi_1_1_buffer.md#variable-m_data)  <br> |
|  size\_t | [**m\_Size**](classaby_1_1rhi_1_1_buffer.md#variable-m_size)  <br> |
|  size\_t | [**m\_Stride**](classaby_1_1rhi_1_1_buffer.md#variable-m_stride)  <br> |
























































## Public Functions Documentation




### function IndexBuffer 

```C++
aby::rhi::vulkan::IndexBuffer::IndexBuffer (
    size_t size
) 
```




<hr>



### function destroy 

_Cleanup all resources._ 
```C++
virtual void aby::rhi::vulkan::IndexBuffer::destroy () override
```



Implements [*aby::rhi::IndexBuffer::destroy*](classaby_1_1rhi_1_1_index_buffer.md#function-destroy)


<hr>



### function gpu 

```C++
vulkan::Buffer & aby::rhi::vulkan::IndexBuffer::gpu () 
```




<hr>



### function upload 

_Upload the buffers cpu data to the GPU._ 
```C++
virtual void aby::rhi::vulkan::IndexBuffer::upload () override
```



Implements [*aby::rhi::IndexBuffer::upload*](classaby_1_1rhi_1_1_index_buffer.md#function-upload)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-buffer.hpp`

