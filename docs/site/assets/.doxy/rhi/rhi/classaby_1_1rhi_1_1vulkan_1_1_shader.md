

# Class aby::rhi::vulkan::Shader



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Shader**](classaby_1_1rhi_1_1vulkan_1_1_shader.md)








Inherits the following classes: [aby::rhi::Shader](classaby_1_1rhi_1_1_shader.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Shader**](#function-shader) (EShader type, const std::vector&lt; uint32\_t &gt; & data) <br> |
| virtual void | [**bind**](#function-bind) () override<br> |
| virtual std::span&lt; uint32\_t &gt; | [**data**](#function-data) () <br> |
| virtual void | [**destroy**](#function-destroy) () override<br> |
|  vk::ShaderModule | [**module**](#function-module) () <br> |
| virtual EShader | [**type**](#function-type) () override const<br> |
| virtual  | [**~Shader**](#function-shader) () <br> |


## Public Functions inherited from aby::rhi::Shader

See [aby::rhi::Shader](classaby_1_1rhi_1_1_shader.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**bind**](classaby_1_1rhi_1_1_shader.md#function-bind) () = 0<br> |
| virtual std::span&lt; uint32\_t &gt; | [**data**](classaby_1_1rhi_1_1_shader.md#function-data) () = 0<br> |
| virtual void | [**destroy**](classaby_1_1rhi_1_1_shader.md#function-destroy) () = 0<br> |
| virtual EShader | [**type**](classaby_1_1rhi_1_1_shader.md#function-type) () const = 0<br> |
| virtual  | [**~Shader**](classaby_1_1rhi_1_1_shader.md#function-shader) () = default<br> |




## Public Static Functions inherited from aby::rhi::Shader

See [aby::rhi::Shader](classaby_1_1rhi_1_1_shader.md)

| Type | Name |
| ---: | :--- |
|  [**Resource**](classaby_1_1rhi_1_1_resource.md) | [**create**](classaby_1_1rhi_1_1_shader.md#function-create) (const fs::path & rel\_path) <br>_The type of shader will be determined by the path._  |
















































## Protected Static Functions inherited from aby::rhi::Shader

See [aby::rhi::Shader](classaby_1_1rhi_1_1_shader.md)

| Type | Name |
| ---: | :--- |
|  EShader | [**ext\_to\_eshader**](classaby_1_1rhi_1_1_shader.md#function-ext_to_eshader) (const fs::path & ext) <br> |
|  [**PathData**](structaby_1_1rhi_1_1_shader_1_1_path_data.md) | [**get\_path\_data**](classaby_1_1rhi_1_1_shader.md#function-get_path_data) (const fs::path & rel\_path) <br> |
|  bool | [**is\_cached\_shader**](classaby_1_1rhi_1_1_shader.md#function-is_cached_shader) (fs::path rel\_path) <br> |
|  size\_t | [**size\_of\_glsl\_type**](classaby_1_1rhi_1_1_shader.md#function-size_of_glsl_type) (const std::string & glsl\_type) <br> |


## Public Functions Documentation




### function Shader 

```C++
aby::rhi::vulkan::Shader::Shader (
    EShader type,
    const std::vector< uint32_t > & data
) 
```




<hr>



### function bind 

```C++
virtual void aby::rhi::vulkan::Shader::bind () override
```



Implements [*aby::rhi::Shader::bind*](classaby_1_1rhi_1_1_shader.md#function-bind)


<hr>



### function data 

```C++
virtual std::span< uint32_t > aby::rhi::vulkan::Shader::data () 
```



Implements [*aby::rhi::Shader::data*](classaby_1_1rhi_1_1_shader.md#function-data)


<hr>



### function destroy 

```C++
virtual void aby::rhi::vulkan::Shader::destroy () override
```



Implements [*aby::rhi::Shader::destroy*](classaby_1_1rhi_1_1_shader.md#function-destroy)


<hr>



### function module 

```C++
vk::ShaderModule aby::rhi::vulkan::Shader::module () 
```




<hr>



### function type 

```C++
virtual EShader aby::rhi::vulkan::Shader::type () override const
```



Implements [*aby::rhi::Shader::type*](classaby_1_1rhi_1_1_shader.md#function-type)


<hr>



### function ~Shader 

```C++
virtual aby::rhi::vulkan::Shader::~Shader () 
```



Implements [*aby::rhi::Shader::~Shader*](classaby_1_1rhi_1_1_shader.md#function-shader)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-shader.hpp`

