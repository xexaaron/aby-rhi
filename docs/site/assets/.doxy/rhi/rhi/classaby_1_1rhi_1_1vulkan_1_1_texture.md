

# Class aby::rhi::vulkan::Texture



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Texture**](classaby_1_1rhi_1_1vulkan_1_1_texture.md)








Inherits the following classes: [aby::rhi::Texture](classaby_1_1rhi_1_1_texture.md)




























## Public Static Attributes inherited from aby::rhi::Texture

See [aby::rhi::Texture](classaby_1_1rhi_1_1_texture.md)

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**INVALID\_ID**](classaby_1_1rhi_1_1_texture.md#variable-invalid_id)   = `UINT32\_MAX`<br> |


























## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Texture**](#function-texture-12) (ResourceID id, const fs::path & path, const [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & params) <br> |
|   | [**Texture**](#function-texture-22) (ResourceID id, uint32\_t width, uint32\_t height, uint8\_t channels, vk::SampleCountFlagBits samples=vk::SampleCountFlagBits::e1) <br> |
| virtual size\_t | [**bytes**](#function-bytes) () override<br> |
| virtual uint32\_t | [**channels**](#function-channels) () override<br> |
| virtual uint8\_t \*override | [**data**](#function-data) () <br> |
| virtual void | [**destroy**](#function-destroy) () override<br> |
|  vk::Format | [**format**](#function-format) () <br> |
| virtual uint32\_t | [**height**](#function-height) () override<br> |
| virtual uint32\_t | [**id**](#function-id) () override<br>_An id to the texture within an array or descriptor set that can be sent to the GPU to access bindless textures._  |
|  [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) & | [**image**](#function-image) () <br> |
|  bool | [**is\_render\_target**](#function-is_render_target) () const<br> |
| virtual vec4&lt; uint8\_t &gt; | [**read\_px**](#function-read_px) (uint32\_t x, uint32\_t y) override<br>_Read a pixel from the texture's CPU side pixel buffer For render targets read\_pxs() must have been called for the current frame before calling this function. For textures created from a file it will read directly_  _from the cpu side buffer._ |
| virtual void | [**resize**](#function-resize) (uint32\_t w, uint32\_t h) override<br>_Resize the texture._  |
| virtual void | [**sync**](#function-sync) () override<br>_Synchronizes the texture's GPU data with its CPU buffer For render targets this performs a GPU to CPU readback. For textures created from a file it will read directly from the cpu buffer._  |
|  vk::ImageView | [**view**](#function-view) () <br> |
| virtual uint32\_t | [**width**](#function-width) () override<br> |
| virtual void | [**write**](#function-write) (const fs::path & rel\_path) override<br>_Write the image to a file._  |
| virtual  | [**~Texture**](#function-texture) () <br> |


## Public Functions inherited from aby::rhi::Texture

See [aby::rhi::Texture](classaby_1_1rhi_1_1_texture.md)

| Type | Name |
| ---: | :--- |
| virtual size\_t | [**bytes**](classaby_1_1rhi_1_1_texture.md#function-bytes) () = 0<br> |
| virtual uint32\_t | [**channels**](classaby_1_1rhi_1_1_texture.md#function-channels) () = 0<br> |
| virtual uint8\_t \* | [**data**](classaby_1_1rhi_1_1_texture.md#function-data) () = 0<br> |
| virtual void | [**destroy**](classaby_1_1rhi_1_1_texture.md#function-destroy) () = 0<br> |
| virtual uint32\_t | [**height**](classaby_1_1rhi_1_1_texture.md#function-height) () = 0<br> |
| virtual uint32\_t | [**id**](classaby_1_1rhi_1_1_texture.md#function-id) () = 0<br>_An id to the texture within an array or descriptor set that can be sent to the GPU to access bindless textures._  |
| virtual vec4&lt; uint8\_t &gt; | [**read\_px**](classaby_1_1rhi_1_1_texture.md#function-read_px) (uint32\_t x, uint32\_t y) = 0<br>_Read a pixel from the texture's CPU side pixel buffer For render targets read\_pxs() must have been called for the current frame before calling this function. For textures created from a file it will read directly_  _from the cpu side buffer._ |
| virtual void | [**resize**](classaby_1_1rhi_1_1_texture.md#function-resize) (uint32\_t w, uint32\_t h) = 0<br>_Resize the texture._  |
| virtual void | [**sync**](classaby_1_1rhi_1_1_texture.md#function-sync) () = 0<br>_Synchronizes the texture's GPU data with its CPU buffer For render targets this performs a GPU to CPU readback. For textures created from a file it will read directly from the cpu buffer._  |
| virtual uint32\_t | [**width**](classaby_1_1rhi_1_1_texture.md#function-width) () = 0<br> |
| virtual void | [**write**](classaby_1_1rhi_1_1_texture.md#function-write) (const fs::path & rel\_path) = 0<br>_Write the image to a file._  |
| virtual  | [**~Texture**](classaby_1_1rhi_1_1_texture.md#function-texture) () = default<br> |




## Public Static Functions inherited from aby::rhi::Texture

See [aby::rhi::Texture](classaby_1_1rhi_1_1_texture.md)

| Type | Name |
| ---: | :--- |
|  [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; [**Texture**](classaby_1_1rhi_1_1_texture.md), EResource::texture &gt; | [**create**](classaby_1_1rhi_1_1_texture.md#function-create) (const fs::path & rel\_path, const [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & params={}) <br> |
|  [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)&lt; [**Texture**](classaby_1_1rhi_1_1_texture.md), EResource::texture &gt; | [**create\_render\_target**](classaby_1_1rhi_1_1_texture.md#function-create_render_target) (uint8\_t channels, EAntiAliasing aliasing=EAntiAliasing::none) <br> |


















































## Public Functions Documentation




### function Texture [1/2]

```C++
aby::rhi::vulkan::Texture::Texture (
    ResourceID id,
    const fs::path & path,
    const TextureParams & params
) 
```




<hr>



### function Texture [2/2]

```C++
aby::rhi::vulkan::Texture::Texture (
    ResourceID id,
    uint32_t width,
    uint32_t height,
    uint8_t channels,
    vk::SampleCountFlagBits samples=vk::SampleCountFlagBits::e1
) 
```




<hr>



### function bytes 

```C++
virtual size_t aby::rhi::vulkan::Texture::bytes () override
```



Implements [*aby::rhi::Texture::bytes*](classaby_1_1rhi_1_1_texture.md#function-bytes)


<hr>



### function channels 

```C++
virtual uint32_t aby::rhi::vulkan::Texture::channels () override
```



Implements [*aby::rhi::Texture::channels*](classaby_1_1rhi_1_1_texture.md#function-channels)


<hr>



### function data 

```C++
virtual uint8_t *override aby::rhi::vulkan::Texture::data () 
```



Implements [*aby::rhi::Texture::data*](classaby_1_1rhi_1_1_texture.md#function-data)


<hr>



### function destroy 

```C++
virtual void aby::rhi::vulkan::Texture::destroy () override
```



Implements [*aby::rhi::Texture::destroy*](classaby_1_1rhi_1_1_texture.md#function-destroy)


<hr>



### function format 

```C++
vk::Format aby::rhi::vulkan::Texture::format () 
```




<hr>



### function height 

```C++
virtual uint32_t aby::rhi::vulkan::Texture::height () override
```



Implements [*aby::rhi::Texture::height*](classaby_1_1rhi_1_1_texture.md#function-height)


<hr>



### function id 

_An id to the texture within an array or descriptor set that can be sent to the GPU to access bindless textures._ 
```C++
virtual uint32_t aby::rhi::vulkan::Texture::id () override
```



Implements [*aby::rhi::Texture::id*](classaby_1_1rhi_1_1_texture.md#function-id)


<hr>



### function image 

```C++
Image & aby::rhi::vulkan::Texture::image () 
```




<hr>



### function is\_render\_target 

```C++
bool aby::rhi::vulkan::Texture::is_render_target () const
```




<hr>



### function read\_px 

_Read a pixel from the texture's CPU side pixel buffer For render targets read\_pxs() must have been called for the current frame before calling this function. For textures created from a file it will read directly_  _from the cpu side buffer._
```C++
virtual vec4< uint8_t > aby::rhi::vulkan::Texture::read_px (
    uint32_t x,
    uint32_t y
) override
```





**Parameters:**


* `x` The x position 
* `y` The y position 



**Returns:**

CPU side rgba8 pixel data 





        
Implements [*aby::rhi::Texture::read\_px*](classaby_1_1rhi_1_1_texture.md#function-read_px)


<hr>



### function resize 

_Resize the texture._ 
```C++
virtual void aby::rhi::vulkan::Texture::resize (
    uint32_t w,
    uint32_t h
) override
```





**Parameters:**


* `w` new width 
* `h` new height 




        
Implements [*aby::rhi::Texture::resize*](classaby_1_1rhi_1_1_texture.md#function-resize)


<hr>



### function sync 

_Synchronizes the texture's GPU data with its CPU buffer For render targets this performs a GPU to CPU readback. For textures created from a file it will read directly from the cpu buffer._ 
```C++
virtual void aby::rhi::vulkan::Texture::sync () override
```





**Note:**

For render targets this must be called once per frame before calling [**read\_px()**](classaby_1_1rhi_1_1vulkan_1_1_texture.md#function-read_px) if the current frames pixels are required 





        
Implements [*aby::rhi::Texture::sync*](classaby_1_1rhi_1_1_texture.md#function-sync)


<hr>



### function view 

```C++
vk::ImageView aby::rhi::vulkan::Texture::view () 
```




<hr>



### function width 

```C++
virtual uint32_t aby::rhi::vulkan::Texture::width () override
```



Implements [*aby::rhi::Texture::width*](classaby_1_1rhi_1_1_texture.md#function-width)


<hr>



### function write 

_Write the image to a file._ 
```C++
virtual void aby::rhi::vulkan::Texture::write (
    const fs::path & rel_path
) override
```





**Parameters:**


* `rel_path` Output image type determined by path extension. 




        
Implements [*aby::rhi::Texture::write*](classaby_1_1rhi_1_1_texture.md#function-write)


<hr>



### function ~Texture 

```C++
virtual aby::rhi::vulkan::Texture::~Texture () 
```



Implements [*aby::rhi::Texture::~Texture*](classaby_1_1rhi_1_1_texture.md#function-texture)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-texture.hpp`

