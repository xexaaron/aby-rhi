

# Class aby::rhi::vulkan::DescriptorLayoutBuilder



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md) & | [**add\_binding**](#function-add_binding) (uint32\_t binding, vk::DescriptorType type) <br> |
|  vk::DescriptorSetLayout | [**build**](#function-build) () <br> |
|  void | [**clear**](#function-clear) () <br> |
|  [**DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md) & | [**set\_flags**](#function-set_flags) (vk::DescriptorSetLayoutCreateFlags flags) <br> |
|  [**DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md) & | [**set\_pnext**](#function-set_pnext) (void \* pnext) <br> |
|  [**DescriptorLayoutBuilder**](classaby_1_1rhi_1_1vulkan_1_1_descriptor_layout_builder.md) & | [**set\_stages**](#function-set_stages) (vk::ShaderStageFlags flags) <br> |




























## Public Functions Documentation




### function add\_binding 

```C++
DescriptorLayoutBuilder & aby::rhi::vulkan::DescriptorLayoutBuilder::add_binding (
    uint32_t binding,
    vk::DescriptorType type
) 
```




<hr>



### function build 

```C++
vk::DescriptorSetLayout aby::rhi::vulkan::DescriptorLayoutBuilder::build () 
```




<hr>



### function clear 

```C++
void aby::rhi::vulkan::DescriptorLayoutBuilder::clear () 
```




<hr>



### function set\_flags 

```C++
DescriptorLayoutBuilder & aby::rhi::vulkan::DescriptorLayoutBuilder::set_flags (
    vk::DescriptorSetLayoutCreateFlags flags
) 
```




<hr>



### function set\_pnext 

```C++
DescriptorLayoutBuilder & aby::rhi::vulkan::DescriptorLayoutBuilder::set_pnext (
    void * pnext
) 
```




<hr>



### function set\_stages 

```C++
DescriptorLayoutBuilder & aby::rhi::vulkan::DescriptorLayoutBuilder::set_stages (
    vk::ShaderStageFlags flags
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-descriptors.hpp`

