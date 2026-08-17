

# Struct aby::rhi::TextureParams



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  float | [**anisotropy\_filtering**](#variable-anisotropy_filtering)   = `0.f`<br> |
|  EChannels | [**channels**](#variable-channels)   = `EChannels::rgba`<br> |
|  EFiltering | [**filtering**](#variable-filtering)   = `EFiltering::linear`<br> |
|  uint32\_t | [**mip\_levels**](#variable-mip_levels)   = `0`<br> |
|  ERepeatMode | [**repeat\_mode**](#variable-repeat_mode)   = `ERepeatMode::repeat`<br> |
|  ETextureUsage | [**texture\_usage**](#variable-texture_usage)   = `ETextureUsage::albedo`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & | [**set\_anisotropy\_filtering**](#function-set_anisotropy_filtering) (float anisotropy) <br> |
|  [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & | [**set\_channels**](#function-set_channels) (EChannels channels) <br> |
|  [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & | [**set\_filtering**](#function-set_filtering) (EFiltering filtering) <br> |
|  [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & | [**set\_mip\_levels**](#function-set_mip_levels) (uint32\_t mip\_levels) <br> |
|  [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & | [**set\_repeat\_mode**](#function-set_repeat_mode) (ERepeatMode repeat\_mode) <br> |
|  [**TextureParams**](structaby_1_1rhi_1_1_texture_params.md) & | [**set\_texture\_usage**](#function-set_texture_usage) (ETextureUsage texture\_usage) <br> |




























## Public Attributes Documentation




### variable anisotropy\_filtering 

```C++
float aby::rhi::TextureParams::anisotropy_filtering;
```




<hr>



### variable channels 

```C++
EChannels aby::rhi::TextureParams::channels;
```




<hr>



### variable filtering 

```C++
EFiltering aby::rhi::TextureParams::filtering;
```




<hr>



### variable mip\_levels 

```C++
uint32_t aby::rhi::TextureParams::mip_levels;
```




<hr>



### variable repeat\_mode 

```C++
ERepeatMode aby::rhi::TextureParams::repeat_mode;
```




<hr>



### variable texture\_usage 

```C++
ETextureUsage aby::rhi::TextureParams::texture_usage;
```




<hr>
## Public Functions Documentation




### function set\_anisotropy\_filtering 

```C++
TextureParams & aby::rhi::TextureParams::set_anisotropy_filtering (
    float anisotropy
) 
```




<hr>



### function set\_channels 

```C++
TextureParams & aby::rhi::TextureParams::set_channels (
    EChannels channels
) 
```




<hr>



### function set\_filtering 

```C++
TextureParams & aby::rhi::TextureParams::set_filtering (
    EFiltering filtering
) 
```




<hr>



### function set\_mip\_levels 

```C++
TextureParams & aby::rhi::TextureParams::set_mip_levels (
    uint32_t mip_levels
) 
```




<hr>



### function set\_repeat\_mode 

```C++
TextureParams & aby::rhi::TextureParams::set_repeat_mode (
    ERepeatMode repeat_mode
) 
```




<hr>



### function set\_texture\_usage 

```C++
TextureParams & aby::rhi::TextureParams::set_texture_usage (
    ETextureUsage texture_usage
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/texture.hpp`

