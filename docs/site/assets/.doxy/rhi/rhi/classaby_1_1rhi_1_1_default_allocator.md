

# Class aby::rhi::DefaultAllocator



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**DefaultAllocator**](classaby_1_1rhi_1_1_default_allocator.md)








Inherits the following classes: [aby::rhi::IAllocator](classaby_1_1rhi_1_1_i_allocator.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void \*override | [**alloc**](#function-alloc) (size\_t bytes, size\_t alignment, EAllocation type) <br> |
| virtual void | [**free**](#function-free) (void \* ptr, EAllocation type) override<br> |
| virtual std::string\_view | [**name**](#function-name) () override<br> |
| virtual void \*override | [**realloc**](#function-realloc) (void \* ptr, size\_t bytes, size\_t alignment, EAllocation type) <br> |


## Public Functions inherited from aby::rhi::IAllocator

See [aby::rhi::IAllocator](classaby_1_1rhi_1_1_i_allocator.md)

| Type | Name |
| ---: | :--- |
| virtual void \* | [**alloc**](classaby_1_1rhi_1_1_i_allocator.md#function-alloc) (size\_t bytes, size\_t alignment, EAllocation type) = 0<br> |
| virtual void | [**free**](classaby_1_1rhi_1_1_i_allocator.md#function-free) (void \* ptr, EAllocation type) = 0<br> |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_allocator.md#function-name) () override<br> |
| virtual void \* | [**realloc**](classaby_1_1rhi_1_1_i_allocator.md#function-realloc) (void \* ptr, size\_t bytes, size\_t alignment, EAllocation type) = 0<br> |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |
















































































## Public Functions Documentation




### function alloc 

```C++
virtual void *override aby::rhi::DefaultAllocator::alloc (
    size_t bytes,
    size_t alignment,
    EAllocation type
) 
```



Implements [*aby::rhi::IAllocator::alloc*](classaby_1_1rhi_1_1_i_allocator.md#function-alloc)


<hr>



### function free 

```C++
virtual void aby::rhi::DefaultAllocator::free (
    void * ptr,
    EAllocation type
) override
```



Implements [*aby::rhi::IAllocator::free*](classaby_1_1rhi_1_1_i_allocator.md#function-free)


<hr>



### function name 

```C++
virtual std::string_view aby::rhi::DefaultAllocator::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>



### function realloc 

```C++
virtual void *override aby::rhi::DefaultAllocator::realloc (
    void * ptr,
    size_t bytes,
    size_t alignment,
    EAllocation type
) 
```



Implements [*aby::rhi::IAllocator::realloc*](classaby_1_1rhi_1_1_i_allocator.md#function-realloc)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/default_allocator.hpp`

