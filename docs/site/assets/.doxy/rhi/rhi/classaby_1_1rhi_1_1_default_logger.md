

# Class aby::rhi::DefaultLogger



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**DefaultLogger**](classaby_1_1rhi_1_1_default_logger.md)








Inherits the following classes: [aby::rhi::ILogger](classaby_1_1rhi_1_1_i_logger.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**log**](#function-log) (ELogLevel level, const std::string & msg) override<br> |
| virtual std::string\_view | [**name**](#function-name) () override<br> |


## Public Functions inherited from aby::rhi::ILogger

See [aby::rhi::ILogger](classaby_1_1rhi_1_1_i_logger.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**log**](classaby_1_1rhi_1_1_i_logger.md#function-log) (ELogLevel level, const std::string & msg) = 0<br> |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_logger.md#function-name) () override<br> |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |
















































































## Public Functions Documentation




### function log 

```C++
virtual void aby::rhi::DefaultLogger::log (
    ELogLevel level,
    const std::string & msg
) override
```



Implements [*aby::rhi::ILogger::log*](classaby_1_1rhi_1_1_i_logger.md#function-log)


<hr>



### function name 

```C++
virtual std::string_view aby::rhi::DefaultLogger::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/default_logger.hpp`

