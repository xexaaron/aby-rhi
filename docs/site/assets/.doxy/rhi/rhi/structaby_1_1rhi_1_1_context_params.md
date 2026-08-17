

# Struct aby::rhi::ContextParams



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**ContextParams**](structaby_1_1rhi_1_1_context_params.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) | [**graphics**](#variable-graphics)  <br>_The graphics configuraton to pass to the renderer._  |
|  void \* | [**native\_window**](#variable-native_window)  <br>_The native window handle(s)_  |
|  ERenderer | [**renderer\_backend**](#variable-renderer_backend)  <br>_The renderer backend to create._  |
|  EWindow | [**window\_backend**](#variable-window_backend)  <br>_The window backend to use (provided because their are multiple windowing backends on linux)_  |












































## Public Attributes Documentation




### variable graphics 

_The graphics configuraton to pass to the renderer._ 
```C++
GraphicsParams aby::rhi::ContextParams::graphics;
```




<hr>



### variable native\_window 

_The native window handle(s)_ 
```C++
void* aby::rhi::ContextParams::native_window;
```





**Parameters:**


* `Win32` HWND 
* `X11` std::pair&lt;Display\*, Window&gt; 
* `XCB` std::pair&lt;xcb\_connection\_t\*, xcb\_window\_t&gt; 
* `Wayland` std::pair&lt;wl\_display\*, wl\_surface\*&gt; 
* `Metal` CAMetalLayer\* 




        

<hr>



### variable renderer\_backend 

_The renderer backend to create._ 
```C++
ERenderer aby::rhi::ContextParams::renderer_backend;
```




<hr>



### variable window\_backend 

_The window backend to use (provided because their are multiple windowing backends on linux)_ 
```C++
EWindow aby::rhi::ContextParams::window_backend;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/context.hpp`

