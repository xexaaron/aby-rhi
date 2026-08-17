

# Class aby::rhi::detail::ScopedTimer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**detail**](namespaceaby_1_1rhi_1_1detail.md) **>** [**ScopedTimer**](classaby_1_1rhi_1_1detail_1_1_scoped_timer.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::chrono::steady\_clock | [**Clock**](#typedef-clock)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ScopedTimer**](#function-scopedtimer) (std::string\_view name) <br> |
|   | [**~ScopedTimer**](#function-scopedtimer) () <br> |




























## Public Types Documentation




### typedef Clock 

```C++
using aby::rhi::detail::ScopedTimer::Clock =  std::chrono::steady_clock;
```




<hr>
## Public Functions Documentation




### function ScopedTimer 

```C++
explicit aby::rhi::detail::ScopedTimer::ScopedTimer (
    std::string_view name
) 
```




<hr>



### function ~ScopedTimer 

```C++
aby::rhi::detail::ScopedTimer::~ScopedTimer () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/common.hpp`

