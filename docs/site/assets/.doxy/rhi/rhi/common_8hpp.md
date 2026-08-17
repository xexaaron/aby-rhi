

# File common.hpp



[**FileList**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**common.hpp**](common_8hpp.md)

[Go to the source code of this file](common_8hpp_source.md)



* `#include "common-enums.hpp"`
* `#include "common-types.hpp"`
* `#include <chrono>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**aby**](namespaceaby.md) <br> |
| namespace | [**rhi**](namespaceaby_1_1rhi.md) <br>_Operators for enum classes._  |
| namespace | [**detail**](namespaceaby_1_1rhi_1_1detail.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**ScopedTimer**](classaby_1_1rhi_1_1detail_1_1_scoped_timer.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ABY\_RHI\_DEBUG\_BREAK**](common_8hpp.md#define-aby_rhi_debug_break) () <br> |
| define  | [**ABY\_RHI\_FUNCTION\_NAME**](common_8hpp.md#define-aby_rhi_function_name)  `\_\_func\_\_`<br> |
| define  | [**aby\_rhi\_assert**](common_8hpp.md#define-aby_rhi_assert) (expr, ...) `/* multi line expression */`<br> |
| define  | [**aby\_rhi\_dbg**](common_8hpp.md#define-aby_rhi_dbg) (msg, ...) `[**::aby::rhi::Context::get**](classaby_1_1rhi_1_1_context.md#function-get)().logger()-&gt;log(::aby::rhi::ELogLevel::debug, std::format(msg \_\_VA\_OPT\_\_(, ) \_\_VA\_ARGS\_\_))`<br> |
| define  | [**aby\_rhi\_err**](common_8hpp.md#define-aby_rhi_err) (msg, ...) `[**::aby::rhi::Context::get**](classaby_1_1rhi_1_1_context.md#function-get)().logger()-&gt;log(::aby::rhi::ELogLevel::error, std::format(msg \_\_VA\_OPT\_\_(, ) \_\_VA\_ARGS\_\_))`<br> |
| define  | [**aby\_rhi\_ftl**](common_8hpp.md#define-aby_rhi_ftl) (msg, ...) `[**::aby::rhi::Context::get**](classaby_1_1rhi_1_1_context.md#function-get)().logger()-&gt;log(::aby::rhi::ELogLevel::fatal, std::format(msg \_\_VA\_OPT\_\_(, ) \_\_VA\_ARGS\_\_))`<br> |
| define  | [**aby\_rhi\_log**](common_8hpp.md#define-aby_rhi_log) (...) <br> |
| define  | [**aby\_rhi\_profile**](common_8hpp.md#define-aby_rhi_profile) (...) <br> |
| define  | [**aby\_rhi\_trc**](common_8hpp.md#define-aby_rhi_trc) (...) <br> |
| define  | [**aby\_rhi\_wrn**](common_8hpp.md#define-aby_rhi_wrn) (...) <br> |

## Macro Definition Documentation





### define ABY\_RHI\_DEBUG\_BREAK 

```C++
#define ABY_RHI_DEBUG_BREAK (
    
) 
```




<hr>



### define ABY\_RHI\_FUNCTION\_NAME 

```C++
#define ABY_RHI_FUNCTION_NAME `__func__`
```




<hr>



### define aby\_rhi\_assert 

```C++
#define aby_rhi_assert (
    expr,
    ...
) `/* multi line expression */`
```




<hr>



### define aby\_rhi\_dbg 

```C++
#define aby_rhi_dbg (
    msg,
    ...
) `::aby::rhi::Context::get ().logger()->log(::aby::rhi::ELogLevel::debug, std::format(msg __VA_OPT__(, ) __VA_ARGS__))`
```




<hr>



### define aby\_rhi\_err 

```C++
#define aby_rhi_err (
    msg,
    ...
) `::aby::rhi::Context::get ().logger()->log(::aby::rhi::ELogLevel::error, std::format(msg __VA_OPT__(, ) __VA_ARGS__))`
```




<hr>



### define aby\_rhi\_ftl 

```C++
#define aby_rhi_ftl (
    msg,
    ...
) `::aby::rhi::Context::get ().logger()->log(::aby::rhi::ELogLevel::fatal, std::format(msg __VA_OPT__(, ) __VA_ARGS__))`
```




<hr>



### define aby\_rhi\_log 

```C++
#define aby_rhi_log (
    ...
) 
```




<hr>



### define aby\_rhi\_profile 

```C++
#define aby_rhi_profile (
    ...
) 
```




<hr>



### define aby\_rhi\_trc 

```C++
#define aby_rhi_trc (
    ...
) 
```




<hr>



### define aby\_rhi\_wrn 

```C++
#define aby_rhi_wrn (
    ...
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/common.hpp`

