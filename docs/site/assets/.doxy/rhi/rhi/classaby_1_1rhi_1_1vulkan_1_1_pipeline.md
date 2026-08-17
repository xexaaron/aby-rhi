

# Class aby::rhi::vulkan::Pipeline



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Pipeline**](classaby_1_1rhi_1_1vulkan_1_1_pipeline.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Pipeline**](#function-pipeline) (vk::Pipeline pipeline, vk::PipelineLayout layout, const std::vector&lt; vk::DescriptorSet &gt; & sets) <br> |
|  void | [**bind**](#function-bind) (vk::CommandBuffer cmd, vk::PipelineBindPoint point) <br> |
|  void | [**destroy**](#function-destroy) () <br> |
|   | [**operator VkPipeline**](#function-operator-vkpipeline) () <br> |
|   | [**Pipeline**](#function-pipeline) () <br> |




























## Public Functions Documentation




### function Pipeline 

```C++
aby::rhi::vulkan::Pipeline::Pipeline (
    vk::Pipeline pipeline,
    vk::PipelineLayout layout,
    const std::vector< vk::DescriptorSet > & sets
) 
```




<hr>



### function bind 

```C++
void aby::rhi::vulkan::Pipeline::bind (
    vk::CommandBuffer cmd,
    vk::PipelineBindPoint point
) 
```




<hr>



### function destroy 

```C++
void aby::rhi::vulkan::Pipeline::destroy () 
```




<hr>



### function operator VkPipeline 

```C++
explicit aby::rhi::vulkan::Pipeline::operator VkPipeline () 
```




<hr>



### function Pipeline 

```C++
explicit aby::rhi::vulkan::Pipeline::Pipeline () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-pipeline.hpp`

