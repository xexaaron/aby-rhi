

# Class aby::rhi::vulkan::Buffer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Buffer**](#function-buffer-14) () <br> |
|   | [**Buffer**](#function-buffer-24) (size\_t size, vk::BufferUsageFlags usage, VmaMemoryUsage memory\_usage) <br> |
|   | [**Buffer**](#function-buffer-34) (const [**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) & other) <br> |
|   | [**Buffer**](#function-buffer-44) ([**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) && other) <br> |
|  VmaAllocation | [**allocation**](#function-allocation) () <br> |
|  VmaAllocationInfo | [**allocation\_info**](#function-allocation_info) () <br> |
|  void | [**clear**](#function-clear) () <br> |
|  bool | [**copy\_to**](#function-copy_to) (vk::Buffer dst, size\_t bytes) <br> |
|  void | [**destroy**](#function-destroy) () <br> |
|   | [**operator VkBuffer**](#function-operator-vkbuffer) () <br> |
|   | [**Buffer**](#function-buffer) () <br> |
|   | [**DeviceAddress**](#function-deviceaddress) () <br> |
|  [**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) & | [**operator=**](#function-operator) ([**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) && other) <br> |
|  void | [**read**](#function-read) (std::vector&lt; uint8\_t &gt; \* out\_data) <br> |
|  void | [**write**](#function-write) (void \* data, size\_t bytes) <br> |
|   | [**~Buffer**](#function-buffer) () <br> |




























## Public Functions Documentation




### function Buffer [1/4]

```C++
aby::rhi::vulkan::Buffer::Buffer () 
```




<hr>



### function Buffer [2/4]

```C++
aby::rhi::vulkan::Buffer::Buffer (
    size_t size,
    vk::BufferUsageFlags usage,
    VmaMemoryUsage memory_usage
) 
```




<hr>



### function Buffer [3/4]

```C++
aby::rhi::vulkan::Buffer::Buffer (
    const Buffer & other
) 
```




<hr>



### function Buffer [4/4]

```C++
aby::rhi::vulkan::Buffer::Buffer (
    Buffer && other
) 
```




<hr>



### function allocation 

```C++
VmaAllocation aby::rhi::vulkan::Buffer::allocation () 
```




<hr>



### function allocation\_info 

```C++
VmaAllocationInfo aby::rhi::vulkan::Buffer::allocation_info () 
```




<hr>



### function clear 

```C++
void aby::rhi::vulkan::Buffer::clear () 
```




<hr>



### function copy\_to 

```C++
bool aby::rhi::vulkan::Buffer::copy_to (
    vk::Buffer dst,
    size_t bytes
) 
```




<hr>



### function destroy 

```C++
void aby::rhi::vulkan::Buffer::destroy () 
```




<hr>



### function operator VkBuffer 

```C++
aby::rhi::vulkan::Buffer::operator VkBuffer () 
```




<hr>



### function Buffer 

```C++
aby::rhi::vulkan::Buffer::Buffer () 
```




<hr>



### function DeviceAddress 

```C++
aby::rhi::vulkan::Buffer::DeviceAddress () 
```




<hr>



### function operator= 

```C++
Buffer & aby::rhi::vulkan::Buffer::operator= (
    Buffer && other
) 
```




<hr>



### function read 

```C++
void aby::rhi::vulkan::Buffer::read (
    std::vector< uint8_t > * out_data
) 
```




<hr>



### function write 

```C++
void aby::rhi::vulkan::Buffer::write (
    void * data,
    size_t bytes
) 
```




<hr>



### function ~Buffer 

```C++
aby::rhi::vulkan::Buffer::~Buffer () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-buffer.hpp`

