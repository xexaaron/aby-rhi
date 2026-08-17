

# Class aby::rhi::Texture



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**Texture**](classaby_1_1rhi_1_1_texture.md)










Inherited by the following classes: [aby::rhi::vulkan::Texture](classaby_1_1rhi_1_1vulkan_1_1_texture.md)


















## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**INVALID\_ID**](#variable-invalid_id)   = `UINT32\_MAX`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
| virtual size\_t | [**bytes**](#function-bytes) () = 0<br> |
| virtual uint32\_t | [**channels**](#function-channels) () = 0<br> |
| virtual uint8\_t \* | [**data**](#function-data) () = 0<br> |
| virtual void | [**destroy**](#function-destroy) () = 0<br> |
| virtual uint32\_t | [**height**](#function-height) () = 0<br> |
| virtual uint32\_t | [**id**](#function-id) () = 0<br>_An id to the texture within an array or descriptor set that can be sent to the GPU to access bindless textures._  |
| virtual vec4&lt; uint8\_t &gt; | [**read\_px**](#function-read_px) (uint32\_t x, uint32\_t y) = 0<br>_Read a pixel from the texture's CPU side pixel buffer For render targets read\_pxs() must have been called for the current frame before calling this function. For textures created from a file it will read directly_  _from the cpu side buffer._ |
| virtual void | [**resize**](#function-resize) (uint32\_t w, uint32\_t h) = 0<br>_Resize the texture._  |
| virtual void | [**sync**](#function-sync) () = 0<br>_Synchronizes the texture's GPU data with its CPU buffer For render targets this performs a GPU to CPU readback. For textures created from a file it will read directly from the cpu buffer._  |
| virtual uint32\_t | [**width**](#function-width) () = 0<br> |
| virtual void | [**write**](#function-write) (const fs::path & rel\_path) = 0<br>_Write the image to a file._  |
| virtual  | [**~Texture**](#function-texture) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; [**Texture**](classaby_1_1rhi_1_1_texture.md), EResource::texture &gt; | [**create**](#function-create) (const fs::path & rel\_path, const [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & params={}) <br> |
|  [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; [**Texture**](classaby_1_1rhi_1_1_texture.md), EResource::texture &gt; | [**create\_render\_target**](#function-create_render_target) (uint8\_t channels, EAntiAliasing aliasing=EAntiAliasing::none) <br> |


























## Public Static Attributes Documentation




### variable INVALID\_ID 

```C++
uint32_t aby::rhi::Texture::INVALID_ID;
```




<hr>
## Public Functions Documentation




### function bytes 

```C++
virtual size_t aby::rhi::Texture::bytes () = 0
```




<hr>



### function channels 

```C++
virtual uint32_t aby::rhi::Texture::channels () = 0
```




<hr>



### function data 

```C++
virtual uint8_t * aby::rhi::Texture::data () = 0
```




<hr>



### function destroy 

```C++
virtual void aby::rhi::Texture::destroy () = 0
```




<hr>



### function height 

```C++
virtual uint32_t aby::rhi::Texture::height () = 0
```




<hr>



### function id 

_An id to the texture within an array or descriptor set that can be sent to the GPU to access bindless textures._ 
```C++
virtual uint32_t aby::rhi::Texture::id () = 0
```




<hr>



### function read\_px 

_Read a pixel from the texture's CPU side pixel buffer For render targets read\_pxs() must have been called for the current frame before calling this function. For textures created from a file it will read directly_  _from the cpu side buffer._
```C++
virtual vec4< uint8_t > aby::rhi::Texture::read_px (
    uint32_t x,
    uint32_t y
) = 0
```





**Parameters:**


* `x` The x position 
* `y` The y position 



**Returns:**

CPU side rgba8 pixel data 





        

<hr>



### function resize 

_Resize the texture._ 
```C++
virtual void aby::rhi::Texture::resize (
    uint32_t w,
    uint32_t h
) = 0
```





**Parameters:**


* `w` new width 
* `h` new height 




        

<hr>



### function sync 

_Synchronizes the texture's GPU data with its CPU buffer For render targets this performs a GPU to CPU readback. For textures created from a file it will read directly from the cpu buffer._ 
```C++
virtual void aby::rhi::Texture::sync () = 0
```





**Note:**

For render targets this must be called once per frame before calling [**read\_px()**](classaby_1_1rhi_1_1_texture.md#function-read_px) if the current frames pixels are required 





        

<hr>



### function width 

```C++
virtual uint32_t aby::rhi::Texture::width () = 0
```




<hr>



### function write 

_Write the image to a file._ 
```C++
virtual void aby::rhi::Texture::write (
    const fs::path & rel_path
) = 0
```





**Parameters:**


* `rel_path` Output image type determined by path extension. 




        

<hr>



### function ~Texture 

```C++
virtual aby::rhi::Texture::~Texture () = default
```




<hr>
## Public Static Functions Documentation




### function create 

```C++
static ResourcePtr < Texture , EResource::texture > aby::rhi::Texture::create (
    const fs::path & rel_path,
    const TextureParams & params={}
) 
```




<hr>



### function create\_render\_target 

```C++
static ResourcePtr < Texture , EResource::texture > aby::rhi::Texture::create_render_target (
    uint8_t channels,
    EAntiAliasing aliasing=EAntiAliasing::none
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/texture.hpp`

