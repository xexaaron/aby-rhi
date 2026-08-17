

# Class aby::rhi::vulkan::GarbageCollector



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**GarbageCollector**](classaby_1_1rhi_1_1vulkan_1_1_garbage_collector.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void()&gt; | [**Dtor**](#typedef-dtor)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**push**](#function-push) (Dtor && dtor) <br> |
|  void | [**run**](#function-run) () <br> |




























## Public Types Documentation




### typedef Dtor 

```C++
using aby::rhi::vulkan::GarbageCollector::Dtor =  std::function<void()>;
```




<hr>
## Public Functions Documentation




### function push 

```C++
void aby::rhi::vulkan::GarbageCollector::push (
    Dtor && dtor
) 
```




<hr>



### function run 

```C++
void aby::rhi::vulkan::GarbageCollector::run () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-gc.hpp`

