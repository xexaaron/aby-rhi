

# Class aby::rhi::DefaultFileIO



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**DefaultFileIO**](classaby_1_1rhi_1_1_default_file_i_o.md)








Inherits the following classes: [aby::rhi::IFileIO](classaby_1_1rhi_1_1_i_file_i_o.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual const fs::path &override | [**cache\_dir**](#function-cache_dir) () const<br> |
| virtual const fs::path &override | [**cwd**](#function-cwd) () const<br> |
| virtual bool | [**read**](#function-read-12) (const fs::path & rel\_path, std::vector&lt; uint8\_t &gt; \* data) override<br> |
| virtual bool | [**read**](#function-read-22) (const fs::path & rel\_path, std::vector&lt; uint32\_t &gt; \* data) override<br> |
| virtual void | [**set\_cache\_dir**](#function-set_cache_dir) (const fs::path & path) override<br> |
| virtual void | [**set\_cwd**](#function-set_cwd) (const fs::path & path) override<br> |
| virtual bool | [**write**](#function-write-12) (const fs::path & rel\_path, std::span&lt; uint8\_t &gt; data) override<br> |
| virtual bool | [**write**](#function-write-22) (const fs::path & rel\_path, std::span&lt; uint32\_t &gt; data) override<br> |


## Public Functions inherited from aby::rhi::IFileIO

See [aby::rhi::IFileIO](classaby_1_1rhi_1_1_i_file_i_o.md)

| Type | Name |
| ---: | :--- |
| virtual const fs::path & | [**cache\_dir**](classaby_1_1rhi_1_1_i_file_i_o.md#function-cache_dir) () const = 0<br> |
| virtual const fs::path & | [**cwd**](classaby_1_1rhi_1_1_i_file_i_o.md#function-cwd) () const = 0<br> |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_file_i_o.md#function-name) () override<br> |
| virtual bool | [**read**](classaby_1_1rhi_1_1_i_file_i_o.md#function-read-12) (const fs::path & rel\_path, std::vector&lt; uint8\_t &gt; \* data) = 0<br> |
| virtual bool | [**read**](classaby_1_1rhi_1_1_i_file_i_o.md#function-read-22) (const fs::path & rel\_path, std::vector&lt; uint32\_t &gt; \* data) = 0<br> |
| virtual void | [**set\_cache\_dir**](classaby_1_1rhi_1_1_i_file_i_o.md#function-set_cache_dir) (const fs::path & path) = 0<br> |
| virtual void | [**set\_cwd**](classaby_1_1rhi_1_1_i_file_i_o.md#function-set_cwd) (const fs::path & path) = 0<br> |
| virtual bool | [**write**](classaby_1_1rhi_1_1_i_file_i_o.md#function-write-12) (const fs::path & rel\_path, std::span&lt; uint8\_t &gt; data) = 0<br> |
| virtual bool | [**write**](classaby_1_1rhi_1_1_i_file_i_o.md#function-write-22) (const fs::path & rel\_path, std::span&lt; uint32\_t &gt; data) = 0<br> |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |
















































































## Public Functions Documentation




### function cache\_dir 

```C++
virtual const fs::path &override aby::rhi::DefaultFileIO::cache_dir () const
```



Implements [*aby::rhi::IFileIO::cache\_dir*](classaby_1_1rhi_1_1_i_file_i_o.md#function-cache_dir)


<hr>



### function cwd 

```C++
virtual const fs::path &override aby::rhi::DefaultFileIO::cwd () const
```



Implements [*aby::rhi::IFileIO::cwd*](classaby_1_1rhi_1_1_i_file_i_o.md#function-cwd)


<hr>



### function read [1/2]

```C++
virtual bool aby::rhi::DefaultFileIO::read (
    const fs::path & rel_path,
    std::vector< uint8_t > * data
) override
```



Implements [*aby::rhi::IFileIO::read*](classaby_1_1rhi_1_1_i_file_i_o.md#function-read-12)


<hr>



### function read [2/2]

```C++
virtual bool aby::rhi::DefaultFileIO::read (
    const fs::path & rel_path,
    std::vector< uint32_t > * data
) override
```



Implements [*aby::rhi::IFileIO::read*](classaby_1_1rhi_1_1_i_file_i_o.md#function-read-22)


<hr>



### function set\_cache\_dir 

```C++
virtual void aby::rhi::DefaultFileIO::set_cache_dir (
    const fs::path & path
) override
```



Implements [*aby::rhi::IFileIO::set\_cache\_dir*](classaby_1_1rhi_1_1_i_file_i_o.md#function-set_cache_dir)


<hr>



### function set\_cwd 

```C++
virtual void aby::rhi::DefaultFileIO::set_cwd (
    const fs::path & path
) override
```



Implements [*aby::rhi::IFileIO::set\_cwd*](classaby_1_1rhi_1_1_i_file_i_o.md#function-set_cwd)


<hr>



### function write [1/2]

```C++
virtual bool aby::rhi::DefaultFileIO::write (
    const fs::path & rel_path,
    std::span< uint8_t > data
) override
```



Implements [*aby::rhi::IFileIO::write*](classaby_1_1rhi_1_1_i_file_i_o.md#function-write-12)


<hr>



### function write [2/2]

```C++
virtual bool aby::rhi::DefaultFileIO::write (
    const fs::path & rel_path,
    std::span< uint32_t > data
) override
```



Implements [*aby::rhi::IFileIO::write*](classaby_1_1rhi_1_1_i_file_i_o.md#function-write-22)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/default_fileio.hpp`

