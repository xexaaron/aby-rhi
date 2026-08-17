

# Class aby::rhi::IFileIO



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**IFileIO**](classaby_1_1rhi_1_1_i_file_i_o.md)



[_**IFileIO**_](classaby_1_1rhi_1_1_i_file_i_o.md) _interface for reading/writing data. Used for reading/writing to/from shaders/textures._[More...](#detailed-description)

* `#include <interfaces.hpp>`



Inherits the following classes: [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)


Inherited by the following classes: [aby::rhi::DefaultFileIO](classaby_1_1rhi_1_1_default_file_i_o.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual const fs::path & | [**cache\_dir**](#function-cache_dir) () const = 0<br> |
| virtual const fs::path & | [**cwd**](#function-cwd) () const = 0<br> |
| virtual std::string\_view | [**name**](#function-name) () override<br> |
| virtual bool | [**read**](#function-read-12) (const fs::path & rel\_path, std::vector&lt; uint8\_t &gt; \* data) = 0<br> |
| virtual bool | [**read**](#function-read-22) (const fs::path & rel\_path, std::vector&lt; uint32\_t &gt; \* data) = 0<br> |
| virtual void | [**set\_cache\_dir**](#function-set_cache_dir) (const fs::path & path) = 0<br> |
| virtual void | [**set\_cwd**](#function-set_cwd) (const fs::path & path) = 0<br> |
| virtual bool | [**write**](#function-write-12) (const fs::path & rel\_path, std::span&lt; uint8\_t &gt; data) = 0<br> |
| virtual bool | [**write**](#function-write-22) (const fs::path & rel\_path, std::span&lt; uint32\_t &gt; data) = 0<br> |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |






















































## Detailed Description




**Parameters:**


* `rel_path` A relative path starting from the currently set working directory 




    
## Public Functions Documentation




### function cache\_dir 

```C++
virtual const fs::path & aby::rhi::IFileIO::cache_dir () const = 0
```




<hr>



### function cwd 

```C++
virtual const fs::path & aby::rhi::IFileIO::cwd () const = 0
```




<hr>



### function name 

```C++
virtual std::string_view aby::rhi::IFileIO::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>



### function read [1/2]

```C++
virtual bool aby::rhi::IFileIO::read (
    const fs::path & rel_path,
    std::vector< uint8_t > * data
) = 0
```




<hr>



### function read [2/2]

```C++
virtual bool aby::rhi::IFileIO::read (
    const fs::path & rel_path,
    std::vector< uint32_t > * data
) = 0
```




<hr>



### function set\_cache\_dir 

```C++
virtual void aby::rhi::IFileIO::set_cache_dir (
    const fs::path & path
) = 0
```




<hr>



### function set\_cwd 

```C++
virtual void aby::rhi::IFileIO::set_cwd (
    const fs::path & path
) = 0
```




<hr>



### function write [1/2]

```C++
virtual bool aby::rhi::IFileIO::write (
    const fs::path & rel_path,
    std::span< uint8_t > data
) = 0
```




<hr>



### function write [2/2]

```C++
virtual bool aby::rhi::IFileIO::write (
    const fs::path & rel_path,
    std::span< uint32_t > data
) = 0
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/interfaces.hpp`

