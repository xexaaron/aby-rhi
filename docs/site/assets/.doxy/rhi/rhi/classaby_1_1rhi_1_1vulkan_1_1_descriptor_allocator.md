

# Class aby::rhi::vulkan::DescriptorAllocator



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**DescriptorAllocator**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_allocator.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  vk::DescriptorSet | [**alloc**](#function-alloc) (vk::DescriptorSetLayout layout) <br> |
|  bool | [**clear**](#function-clear) () <br> |
|  void | [**deinit**](#function-deinit) () <br> |
|  bool | [**init**](#function-init) (uint32\_t max\_sets, std::span&lt; [**PoolSizeRatio**](structaby_1_1rhi_1_1vulkan_1_1_pool_size_ratio.md) &gt; pool\_ratios, vk::DescriptorPoolCreateFlags flags) <br> |




























## Public Functions Documentation




### function alloc 

```C++
vk::DescriptorSet aby::rhi::vulkan::DescriptorAllocator::alloc (
    vk::DescriptorSetLayout layout
) 
```




<hr>



### function clear 

```C++
bool aby::rhi::vulkan::DescriptorAllocator::clear () 
```




<hr>



### function deinit 

```C++
void aby::rhi::vulkan::DescriptorAllocator::deinit () 
```




<hr>



### function init 

```C++
bool aby::rhi::vulkan::DescriptorAllocator::init (
    uint32_t max_sets,
    std::span< PoolSizeRatio > pool_ratios,
    vk::DescriptorPoolCreateFlags flags
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-descriptors.hpp`

