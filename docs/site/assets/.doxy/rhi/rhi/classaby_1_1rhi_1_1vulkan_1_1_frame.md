

# Class aby::rhi::vulkan::Frame



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Frame**](classaby_1_1rhi_1_1vulkan_1_1_frame.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Frame**](#function-frame) () = default<br> |
|  bool | [**begin**](#function-begin) (vk::SwapchainKHR swapchain, uint32\_t \* out\_swapchain\_index) <br> |
|  vk::CommandBuffer | [**cmd**](#function-cmd) () <br> |
|  bool | [**create**](#function-create) (uint32\_t graphics\_queue\_family) <br> |
|  void | [**destroy**](#function-destroy) () <br> |
|  vk::Result | [**end**](#function-end) (vk::SwapchainKHR swapchain, vk::Queue queue, vk::Semaphore render\_finished\_semaphore, uint32\_t swapchain\_index) <br> |
|   | [**~Frame**](#function-frame) () = default<br> |




























## Public Functions Documentation




### function Frame 

```C++
aby::rhi::vulkan::Frame::Frame () = default
```




<hr>



### function begin 

```C++
bool aby::rhi::vulkan::Frame::begin (
    vk::SwapchainKHR swapchain,
    uint32_t * out_swapchain_index
) 
```




<hr>



### function cmd 

```C++
vk::CommandBuffer aby::rhi::vulkan::Frame::cmd () 
```




<hr>



### function create 

```C++
bool aby::rhi::vulkan::Frame::create (
    uint32_t graphics_queue_family
) 
```




<hr>



### function destroy 

```C++
void aby::rhi::vulkan::Frame::destroy () 
```




<hr>



### function end 

```C++
vk::Result aby::rhi::vulkan::Frame::end (
    vk::SwapchainKHR swapchain,
    vk::Queue queue,
    vk::Semaphore render_finished_semaphore,
    uint32_t swapchain_index
) 
```




<hr>



### function ~Frame 

```C++
aby::rhi::vulkan::Frame::~Frame () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-frame.hpp`

