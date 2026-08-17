

# Class aby::rhi::Context



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**Context**](classaby_1_1rhi_1_1_context.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**IAllocator**](classaby_1_1rhi_1_1_i_allocator.md) \* | [**allocator**](#function-allocator) () <br> |
|  void | [**deinit**](#function-deinit) () <br>_Must be called at the end of the application to release all resources._  |
|  [**IFileIO**](classaby_1_1rhi_1_1_i_file_i_o.md) \* | [**file\_io**](#function-file_io) () <br> |
|  bool | [**init**](#function-init) (const [**ContextParams**](structaby_1_1rhi_1_1_context_params.md) & params) <br>_initialize the context. this must be called before anything else relating to the_ [_**aby::rhi**_](namespaceaby_1_1rhi.md) _API_ |
|  [**IJobSystem**](classaby_1_1rhi_1_1_i_job_system.md) \* | [**job\_sys**](#function-job_sys) () <br> |
|  [**ILogger**](classaby_1_1rhi_1_1_i_logger.md) \* | [**logger**](#function-logger) () <br> |
|  [**Renderer**](classaby_1_1rhi_1_1_renderer.md) \* | [**renderer**](#function-renderer) () <br> |
|  ERenderer | [**renderer\_backend**](#function-renderer_backend) () const<br> |
|  void | [**set\_interface**](#function-set_interface) () <br>_Set the interface type. Call this before calling_ [_**Context::init**_](classaby_1_1rhi_1_1_context.md#function-init) _._ |
|  [**ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md)&lt; [**Shader**](classaby_1_1rhi_1_1_shader.md), EResource::shader &gt; & | [**shaders**](#function-shaders) () <br> |
|  [**ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md)&lt; [**Texture**](classaby_1_1rhi_1_1_texture.md), EResource::texture &gt; & | [**textures**](#function-textures) () <br> |
|  EWindow | [**window\_backend**](#function-window_backend) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Context**](classaby_1_1rhi_1_1_context.md) & | [**get**](#function-get) () <br>_Get the static context instance._  |


























## Public Functions Documentation




### function allocator 

```C++
IAllocator * aby::rhi::Context::allocator () 
```




<hr>



### function deinit 

_Must be called at the end of the application to release all resources._ 
```C++
void aby::rhi::Context::deinit () 
```




<hr>



### function file\_io 

```C++
IFileIO * aby::rhi::Context::file_io () 
```




<hr>



### function init 

_initialize the context. this must be called before anything else relating to the_ [_**aby::rhi**_](namespaceaby_1_1rhi.md) _API_
```C++
bool aby::rhi::Context::init (
    const ContextParams & params
) 
```





**Parameters:**


* `context_params` context parameters structure 



**Returns:**

true on success otherwise false 





        

<hr>



### function job\_sys 

```C++
IJobSystem * aby::rhi::Context::job_sys () 
```




<hr>



### function logger 

```C++
ILogger * aby::rhi::Context::logger () 
```




<hr>



### function renderer 

```C++
Renderer * aby::rhi::Context::renderer () 
```




<hr>



### function renderer\_backend 

```C++
ERenderer aby::rhi::Context::renderer_backend () const
```




<hr>



### function set\_interface 

_Set the interface type. Call this before calling_ [_**Context::init**_](classaby_1_1rhi_1_1_context.md#function-init) _._
```C++
template<typename T>
inline void aby::rhi::Context::set_interface () 
```





**Template parameters:**


* [**ILogger**](classaby_1_1rhi_1_1_i_logger.md) Logging interface [optional] 
* [**IAllocator**](classaby_1_1rhi_1_1_i_allocator.md) Allocator interface [optional] 
* [**IFileIO**](classaby_1_1rhi_1_1_i_file_i_o.md) File io interface [optional] 
* [**IJobSystem**](classaby_1_1rhi_1_1_i_job_system.md) Job system interface [optional] 




        

<hr>



### function shaders 

```C++
ResourceContainer < Shader , EResource::shader > & aby::rhi::Context::shaders () 
```




<hr>



### function textures 

```C++
ResourceContainer < Texture , EResource::texture > & aby::rhi::Context::textures () 
```




<hr>



### function window\_backend 

```C++
EWindow aby::rhi::Context::window_backend () const
```




<hr>
## Public Static Functions Documentation




### function get 

_Get the static context instance._ 
```C++
static Context & aby::rhi::Context::get () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/context.hpp`

