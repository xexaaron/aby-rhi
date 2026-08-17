

# Class aby::rhi::Buffer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**Buffer**](classaby_1_1rhi_1_1_buffer.md)










Inherited by the following classes: [aby::rhi::IndexBuffer](classaby_1_1rhi_1_1_index_buffer.md),  [aby::rhi::VertexBuffer](classaby_1_1rhi_1_1_vertex_buffer.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Buffer**](#function-buffer) (size\_t size, size\_t stride) <br>[_**Buffer**_](classaby_1_1rhi_1_1_buffer.md) _constructor._ |
|  size\_t | [**capacity\_bytes**](#function-capacity_bytes) () const<br>_The total amount of bytes._  |
|  void | [**clear**](#function-clear) () <br>_Clears all data from the buffer (CPU ONLY)_  |
|  size\_t | [**count**](#function-count) () const<br>_The number of elements in the buffer._  |
| virtual void | [**destroy**](#function-destroy) () = 0<br>_Cleanup all resources._  |
|  size\_t | [**stride**](#function-stride) () const<br>_The byte size of each element._  |
| virtual void | [**upload**](#function-upload) () = 0<br>_Upload the buffers cpu data to the GPU._  |
|  size\_t | [**used\_bytes**](#function-used_bytes) () const<br>_The amount of bytes already pushed into the buffer._  |
| virtual  | [**~Buffer**](#function-buffer) () = default<br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  size\_t | [**m\_Count**](#variable-m_count)  <br> |
|  uint8\_t \* | [**m\_Data**](#variable-m_data)  <br> |
|  size\_t | [**m\_Size**](#variable-m_size)  <br> |
|  size\_t | [**m\_Stride**](#variable-m_stride)  <br> |




















## Public Functions Documentation




### function Buffer 

[_**Buffer**_](classaby_1_1rhi_1_1_buffer.md) _constructor._
```C++
aby::rhi::Buffer::Buffer (
    size_t size,
    size_t stride
) 
```





**Parameters:**


* `size` The desired element count of the buffer 
* `stride` The byte size of each element in the buffer. 




        

<hr>



### function capacity\_bytes 

_The total amount of bytes._ 
```C++
size_t aby::rhi::Buffer::capacity_bytes () const
```





**Returns:**

[**stride()**](classaby_1_1rhi_1_1_buffer.md#function-stride) \* [**count()**](classaby_1_1rhi_1_1_buffer.md#function-count) 





        

<hr>



### function clear 

_Clears all data from the buffer (CPU ONLY)_ 
```C++
void aby::rhi::Buffer::clear () 
```




<hr>



### function count 

_The number of elements in the buffer._ 
```C++
size_t aby::rhi::Buffer::count () const
```





**Returns:**

[**used\_bytes()**](classaby_1_1rhi_1_1_buffer.md#function-used_bytes) / [**stride()**](classaby_1_1rhi_1_1_buffer.md#function-stride) 





        

<hr>



### function destroy 

_Cleanup all resources._ 
```C++
virtual void aby::rhi::Buffer::destroy () = 0
```




<hr>



### function stride 

_The byte size of each element._ 
```C++
size_t aby::rhi::Buffer::stride () const
```




<hr>



### function upload 

_Upload the buffers cpu data to the GPU._ 
```C++
virtual void aby::rhi::Buffer::upload () = 0
```




<hr>



### function used\_bytes 

_The amount of bytes already pushed into the buffer._ 
```C++
size_t aby::rhi::Buffer::used_bytes () const
```




<hr>



### function ~Buffer 

```C++
virtual aby::rhi::Buffer::~Buffer () = default
```




<hr>
## Protected Attributes Documentation




### variable m\_Count 

```C++
size_t aby::rhi::Buffer::m_Count;
```




<hr>



### variable m\_Data 

```C++
uint8_t* aby::rhi::Buffer::m_Data;
```




<hr>



### variable m\_Size 

```C++
size_t aby::rhi::Buffer::m_Size;
```




<hr>



### variable m\_Stride 

```C++
size_t aby::rhi::Buffer::m_Stride;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/buffer.hpp`

