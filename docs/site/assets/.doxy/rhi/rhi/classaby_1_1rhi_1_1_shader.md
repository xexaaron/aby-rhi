

# Class aby::rhi::Shader



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**Shader**](classaby_1_1rhi_1_1_shader.md)










Inherited by the following classes: [aby::rhi::vulkan::Shader](classaby_1_1rhi_1_1vulkan_1_1_shader.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**bind**](#function-bind) () = 0<br> |
| virtual std::span&lt; uint32\_t &gt; | [**data**](#function-data) () = 0<br> |
| virtual void | [**destroy**](#function-destroy) () = 0<br> |
| virtual EShader | [**type**](#function-type) () const = 0<br> |
| virtual  | [**~Shader**](#function-shader) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Resource**](classaby_1_1rhi_1_1_resource.md) | [**create**](#function-create) (const fs::path & rel\_path) <br>_The type of shader will be determined by the path._  |
























## Protected Static Functions

| Type | Name |
| ---: | :--- |
|  EShader | [**ext\_to\_eshader**](#function-ext_to_eshader) (const fs::path & ext) <br> |
|  [**PathData**](structaby_1_1rhi_1_1_shader_1_1_path_data.md) | [**get\_path\_data**](#function-get_path_data) (const fs::path & rel\_path) <br> |
|  bool | [**is\_cached\_shader**](#function-is_cached_shader) (fs::path rel\_path) <br> |
|  size\_t | [**size\_of\_glsl\_type**](#function-size_of_glsl_type) (const std::string & glsl\_type) <br> |


## Public Functions Documentation




### function bind 

```C++
virtual void aby::rhi::Shader::bind () = 0
```




<hr>



### function data 

```C++
virtual std::span< uint32_t > aby::rhi::Shader::data () = 0
```




<hr>



### function destroy 

```C++
virtual void aby::rhi::Shader::destroy () = 0
```




<hr>



### function type 

```C++
virtual EShader aby::rhi::Shader::type () const = 0
```




<hr>



### function ~Shader 

```C++
virtual aby::rhi::Shader::~Shader () = default
```




<hr>
## Public Static Functions Documentation




### function create 

_The type of shader will be determined by the path._ 
```C++
static Resource aby::rhi::Shader::create (
    const fs::path & rel_path
) 
```





**Warning:**

Do not use precompiled shaders. For vertex shaders we want to be able to parse a description. 




**Parameters:**


* `.geom` Geometry shader 
* `.vert` Vertex shader 
* `.frag` Fragment shader 
* `.comp` Compute shader 




        

<hr>
## Protected Static Functions Documentation




### function ext\_to\_eshader 

```C++
static EShader aby::rhi::Shader::ext_to_eshader (
    const fs::path & ext
) 
```




<hr>



### function get\_path\_data 

```C++
static PathData aby::rhi::Shader::get_path_data (
    const fs::path & rel_path
) 
```




<hr>



### function is\_cached\_shader 

```C++
static bool aby::rhi::Shader::is_cached_shader (
    fs::path rel_path
) 
```




<hr>



### function size\_of\_glsl\_type 

```C++
static size_t aby::rhi::Shader::size_of_glsl_type (
    const std::string & glsl_type
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/shader.hpp`

