

# File vulkan-common.hpp



[**FileList**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-common.hpp**](vulkan-common_8hpp.md)

[Go to the source code of this file](vulkan-common_8hpp_source.md)



* `#include "backends/vulkan/vulkan-callbacks.hpp"`
* `#include "backends/vulkan/vulkan-helpers.hpp"`
* `#include "backends/vulkan/vulkan-platform.hpp"`
* `#include "common.hpp"`
* `#include "context.hpp"`
* `#include <vulkan/vk_enum_string_helper.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**aby**](namespaceaby.md) <br> |
| namespace | [**rhi**](namespaceaby_1_1rhi.md) <br>_Operators for enum classes._  |
| namespace | [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) <br> |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**\_\_vkcheck\_\_**](vulkan-common_8hpp.md#define-__vkcheck__) (result, return\_expression, msg, ...) `/* multi line expression */`<br>_private vulkan check macro that handles different return expressions_  |
| define  | [**vkassert**](vulkan-common_8hpp.md#define-vkassert) (result, msg, ...) `\_\_vkcheck\_\_(result, aby\_rhi\_assert(res == VK\_SUCCESS, "vkassert failed"), msg \_\_VA\_OPT\_\_(, ) \_\_VA\_ARGS\_\_)`<br>_vulkan result check macro that handles different return expressions_  |
| define  | [**vkbcheck**](vulkan-common_8hpp.md#define-vkbcheck) (obj, msg) `/* multi line expression */`<br>_vulkan bootstrap check macro_  |
| define  | [**vkcheck**](vulkan-common_8hpp.md#define-vkcheck) (result, msg, ...) `\_\_vkcheck\_\_(result, return false, msg \_\_VA\_OPT\_\_(, ) \_\_VA\_ARGS\_\_)`<br>_vulkan result check macro that handles different return expressions_  |
| define  | [**vkpropagate**](vulkan-common_8hpp.md#define-vkpropagate) (result, msg, ...) `\_\_vkcheck\_\_(result, return static\_cast&lt;vk::Result&gt;(res), msg \_\_VA\_OPT\_\_(, ) \_\_VA\_ARGS\_\_)`<br>_vulkan result check macro that propogates the error to the caller_  |

## Macro Definition Documentation





### define \_\_vkcheck\_\_ 

_private vulkan check macro that handles different return expressions_ 
```C++
#define __vkcheck__ (
    result,
    return_expression,
    msg,
    ...
) `/* multi line expression */`
```





**Parameters:**


* `result` VkResult from a function or variable 
* `return_expression` What happens when the check fails 
* `msg` formmatable message to log 
* `...` format msg arguments 




        

<hr>



### define vkassert 

_vulkan result check macro that handles different return expressions_ 
```C++
#define vkassert (
    result,
    msg,
    ...
) `__vkcheck__(result, aby_rhi_assert(res == VK_SUCCESS, "vkassert failed"), msg __VA_OPT__(, ) __VA_ARGS__)`
```





**Parameters:**


* `result` VkResult from a function or variable 
* `msg` formmatable message to log 
* `...` format msg arguments 



**Returns:**

assertion on failure. otherwise nothing. 





        

<hr>



### define vkbcheck 

_vulkan bootstrap check macro_ 
```C++
#define vkbcheck (
    obj,
    msg
) `/* multi line expression */`
```





**Parameters:**


* `obj` The Result&lt;T&gt; object from a vkb:: Builder 
* `msg` The message to log as the header for the error 



**Returns:**

false on failure. otherwise nothing 





        

<hr>



### define vkcheck 

_vulkan result check macro that handles different return expressions_ 
```C++
#define vkcheck (
    result,
    msg,
    ...
) `__vkcheck__(result, return false, msg __VA_OPT__(, ) __VA_ARGS__)`
```





**Parameters:**


* `result` VkResult from a function or variable 
* `msg` formmatable message to log 
* `...` format msg arguments 



**Returns:**

false on failure. otherwise nothing. 





        

<hr>



### define vkpropagate 

_vulkan result check macro that propogates the error to the caller_ 
```C++
#define vkpropagate (
    result,
    msg,
    ...
) `__vkcheck__(result, return static_cast<vk::Result>(res), msg __VA_OPT__(, ) __VA_ARGS__)`
```





**Parameters:**


* `result` VkResult from a function or variable 
* `msg` formattable message to log 
* `...` format msg arguments 



**Returns:**

error that was logged, otherwise nothing 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-common.hpp`

