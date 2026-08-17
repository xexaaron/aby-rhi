

# Class aby::rhi::ILogger



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**ILogger**](classaby_1_1rhi_1_1_i_logger.md)



[_**ILogger**_](classaby_1_1rhi_1_1_i_logger.md) _message for passing pre-formatted messages to the application._

* `#include <interfaces.hpp>`



Inherits the following classes: [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)


Inherited by the following classes: [aby::rhi::DefaultLogger](classaby_1_1rhi_1_1_default_logger.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**log**](#function-log) (ELogLevel level, const std::string & msg) = 0<br> |
| virtual std::string\_view | [**name**](#function-name) () override<br> |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |






















































## Public Functions Documentation




### function log 

```C++
virtual void aby::rhi::ILogger::log (
    ELogLevel level,
    const std::string & msg
) = 0
```




<hr>



### function name 

```C++
virtual std::string_view aby::rhi::ILogger::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/interfaces.hpp`

