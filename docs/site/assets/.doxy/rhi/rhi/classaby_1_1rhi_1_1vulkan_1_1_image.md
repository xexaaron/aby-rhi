

# Class aby::rhi::vulkan::Image



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**vulkan**](namespaceaby_1_1rhi_1_1vulkan.md) **>** [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Image**](#function-image-13) () = default<br> |
|   | [**Image**](#function-image-23) (const [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) &) = delete<br> |
|   | [**Image**](#function-image-33) ([**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) && other) noexcept<br> |
|  uint32\_t | [**array\_layers**](#function-array_layers) () const<br> |
|  vk::ImageAspectFlags | [**aspect**](#function-aspect) () const<br> |
|  uint8\_t | [**bpp**](#function-bpp) () const<br> |
|  void | [**copy\_from**](#function-copy_from) (vk::CommandBuffer cmd, [**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) & buffer, uint32\_t mip\_level=0) <br> |
|  void | [**copy\_to**](#function-copy_to-14) (vk::CommandBuffer cmd, vk::Image dst, vk::Extent2D dst\_sz) <br> |
|  void | [**copy\_to**](#function-copy_to-24) (vk::CommandBuffer cmd, [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) & image) <br> |
|  void | [**copy\_to**](#function-copy_to-34) (vk::CommandBuffer cmd, uint32\_t width, uint32\_t height, uint32\_t mip\_level, vk::Filter filter\_mode) <br> |
|  void | [**copy\_to**](#function-copy_to-44) (vk::CommandBuffer cmd, [**Buffer**](classaby_1_1rhi_1_1vulkan_1_1_buffer.md) & buffer, uint32\_t mip\_level=0) <br> |
|  bool | [**create**](#function-create) (vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage, uint32\_t mip\_levels=1) <br> |
|  uint32\_t | [**depth**](#function-depth) () const<br> |
|  void | [**destroy**](#function-destroy) () <br> |
|  const vk::Extent3D & | [**extent**](#function-extent) () const<br> |
|  vk::Extent2D | [**extent2d**](#function-extent2d) () const<br> |
|  const vk::Extent3D & | [**extent3d**](#function-extent3d) () const<br> |
|  vk::Format | [**format**](#function-format) () const<br> |
|  uint32\_t | [**height**](#function-height) () const<br> |
|  vk::Image | [**img**](#function-img) () <br> |
|  bool | [**is\_color**](#function-is_color) () const<br> |
|  bool | [**is\_depth**](#function-is_depth) () const<br> |
|  bool | [**is\_multisampled**](#function-is_multisampled) () const<br> |
|  vk::ImageLayout | [**layout**](#function-layout) (uint32\_t mip\_level=0) const<br> |
|  uint32\_t | [**mip\_levels**](#function-mip_levels) () const<br> |
|   | [**operator VkImage**](#function-operator-vkimage) () <br> |
|   | [**operator VkImageView**](#function-operator-vkimageview) () <br> |
|   | [**operator bool**](#function-operator-bool) () const<br> |
|   | [**Image**](#function-image) () <br> |
|   | [**ImageView**](#function-imageview) () <br> |
|  [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) & | [**operator=**](#function-operator) (const [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) &) = delete<br> |
|  [**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) & | [**operator=**](#function-operator_1) ([**Image**](classaby_1_1rhi_1_1vulkan_1_1_image.md) &&) noexcept<br> |
|  vk::SampleCountFlagBits | [**samples**](#function-samples) () const<br> |
|  void | [**transition**](#function-transition-12) (vk::CommandBuffer cmd, vk::ImageLayout dst\_layout) <br> |
|  void | [**transition**](#function-transition-22) (vk::CommandBuffer cmd, vk::ImageLayout dst\_layout, uint32\_t mip\_level, uint32\_t base\_mip) <br> |
|  vk::ImageUsageFlags | [**usage**](#function-usage) () const<br> |
|  vk::ImageView | [**view**](#function-view) () <br> |
|  uint32\_t | [**width**](#function-width) () const<br> |
|  bool | [**wrap**](#function-wrap) (vk::Image img, vk::ImageView view, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage) <br> |




























## Public Functions Documentation




### function Image [1/3]

```C++
aby::rhi::vulkan::Image::Image () = default
```




<hr>



### function Image [2/3]

```C++
aby::rhi::vulkan::Image::Image (
    const Image &
) = delete
```




<hr>



### function Image [3/3]

```C++
aby::rhi::vulkan::Image::Image (
    Image && other
) noexcept
```




<hr>



### function array\_layers 

```C++
uint32_t aby::rhi::vulkan::Image::array_layers () const
```




<hr>



### function aspect 

```C++
vk::ImageAspectFlags aby::rhi::vulkan::Image::aspect () const
```




<hr>



### function bpp 

```C++
uint8_t aby::rhi::vulkan::Image::bpp () const
```




<hr>



### function copy\_from 

```C++
void aby::rhi::vulkan::Image::copy_from (
    vk::CommandBuffer cmd,
    Buffer & buffer,
    uint32_t mip_level=0
) 
```




<hr>



### function copy\_to [1/4]

```C++
void aby::rhi::vulkan::Image::copy_to (
    vk::CommandBuffer cmd,
    vk::Image dst,
    vk::Extent2D dst_sz
) 
```




<hr>



### function copy\_to [2/4]

```C++
void aby::rhi::vulkan::Image::copy_to (
    vk::CommandBuffer cmd,
    Image & image
) 
```




<hr>



### function copy\_to [3/4]

```C++
void aby::rhi::vulkan::Image::copy_to (
    vk::CommandBuffer cmd,
    uint32_t width,
    uint32_t height,
    uint32_t mip_level,
    vk::Filter filter_mode
) 
```




<hr>



### function copy\_to [4/4]

```C++
void aby::rhi::vulkan::Image::copy_to (
    vk::CommandBuffer cmd,
    Buffer & buffer,
    uint32_t mip_level=0
) 
```




<hr>



### function create 

```C++
bool aby::rhi::vulkan::Image::create (
    vk::Extent3D extent,
    vk::Format format,
    vk::SampleCountFlagBits samples,
    vk::ImageUsageFlags usage,
    uint32_t mip_levels=1
) 
```




<hr>



### function depth 

```C++
uint32_t aby::rhi::vulkan::Image::depth () const
```




<hr>



### function destroy 

```C++
void aby::rhi::vulkan::Image::destroy () 
```




<hr>



### function extent 

```C++
const vk::Extent3D & aby::rhi::vulkan::Image::extent () const
```




<hr>



### function extent2d 

```C++
vk::Extent2D aby::rhi::vulkan::Image::extent2d () const
```




<hr>



### function extent3d 

```C++
const vk::Extent3D & aby::rhi::vulkan::Image::extent3d () const
```




<hr>



### function format 

```C++
vk::Format aby::rhi::vulkan::Image::format () const
```




<hr>



### function height 

```C++
uint32_t aby::rhi::vulkan::Image::height () const
```




<hr>



### function img 

```C++
vk::Image aby::rhi::vulkan::Image::img () 
```




<hr>



### function is\_color 

```C++
bool aby::rhi::vulkan::Image::is_color () const
```




<hr>



### function is\_depth 

```C++
bool aby::rhi::vulkan::Image::is_depth () const
```




<hr>



### function is\_multisampled 

```C++
bool aby::rhi::vulkan::Image::is_multisampled () const
```




<hr>



### function layout 

```C++
vk::ImageLayout aby::rhi::vulkan::Image::layout (
    uint32_t mip_level=0
) const
```




<hr>



### function mip\_levels 

```C++
uint32_t aby::rhi::vulkan::Image::mip_levels () const
```




<hr>



### function operator VkImage 

```C++
aby::rhi::vulkan::Image::operator VkImage () 
```




<hr>



### function operator VkImageView 

```C++
aby::rhi::vulkan::Image::operator VkImageView () 
```




<hr>



### function operator bool 

```C++
explicit aby::rhi::vulkan::Image::operator bool () const
```




<hr>



### function Image 

```C++
aby::rhi::vulkan::Image::Image () 
```




<hr>



### function ImageView 

```C++
aby::rhi::vulkan::Image::ImageView () 
```




<hr>



### function operator= 

```C++
Image & aby::rhi::vulkan::Image::operator= (
    const Image &
) = delete
```




<hr>



### function operator= 

```C++
Image & aby::rhi::vulkan::Image::operator= (
    Image &&
) noexcept
```




<hr>



### function samples 

```C++
vk::SampleCountFlagBits aby::rhi::vulkan::Image::samples () const
```




<hr>



### function transition [1/2]

```C++
void aby::rhi::vulkan::Image::transition (
    vk::CommandBuffer cmd,
    vk::ImageLayout dst_layout
) 
```




<hr>



### function transition [2/2]

```C++
void aby::rhi::vulkan::Image::transition (
    vk::CommandBuffer cmd,
    vk::ImageLayout dst_layout,
    uint32_t mip_level,
    uint32_t base_mip
) 
```




<hr>



### function usage 

```C++
vk::ImageUsageFlags aby::rhi::vulkan::Image::usage () const
```




<hr>



### function view 

```C++
vk::ImageView aby::rhi::vulkan::Image::view () 
```




<hr>



### function width 

```C++
uint32_t aby::rhi::vulkan::Image::width () const
```




<hr>



### function wrap 

```C++
bool aby::rhi::vulkan::Image::wrap (
    vk::Image img,
    vk::ImageView view,
    vk::Extent3D extent,
    vk::Format format,
    vk::SampleCountFlagBits samples,
    vk::ImageUsageFlags usage
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/backends/vulkan/vulkan-image.hpp`

