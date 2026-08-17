

# Class aby::rhi::IAllocator



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**IAllocator**](classaby_1_1rhi_1_1_i_allocator.md)



[_**IAllocator**_](classaby_1_1rhi_1_1_i_allocator.md) _interface for alloc/free/realloc._[More...](#detailed-description)

* `#include <interfaces.hpp>`



Inherits the following classes: [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)


Inherited by the following classes: [aby::rhi::DefaultAllocator](classaby_1_1rhi_1_1_default_allocator.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void \* | [**alloc**](#function-alloc) (size\_t bytes, size\_t alignment, EAllocation type) = 0<br> |
| virtual void | [**free**](#function-free) (void \* ptr, EAllocation type) = 0<br> |
| virtual std::string\_view | [**name**](#function-name) () override<br> |
| virtual void \* | [**realloc**](#function-realloc) (void \* ptr, size\_t bytes, size\_t alignment, EAllocation type) = 0<br> |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |






















































## Detailed Description




**Parameters:**


* `type` can be ignored, it its purely for debuggability/memory usage tracking 
* `alignment` Aligment must be respected at all times. The [**DefaultAllocator**](classaby_1_1rhi_1_1_default_allocator.md) does this already. 




    
## Public Functions Documentation




### function alloc 

```C++
virtual void * aby::rhi::IAllocator::alloc (
    size_t bytes,
    size_t alignment,
    EAllocation type
) = 0
```




<hr>



### function free 

```C++
virtual void aby::rhi::IAllocator::free (
    void * ptr,
    EAllocation type
) = 0
```




<hr>



### function name 

```C++
virtual std::string_view aby::rhi::IAllocator::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>



### function realloc 

```C++
virtual void * aby::rhi::IAllocator::realloc (
    void * ptr,
    size_t bytes,
    size_t alignment,
    EAllocation type
) = 0
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/interfaces.hpp`

