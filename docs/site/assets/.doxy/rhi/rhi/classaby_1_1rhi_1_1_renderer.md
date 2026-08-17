

# Class aby::rhi::Renderer



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**Renderer**](classaby_1_1rhi_1_1_renderer.md)










Inherited by the following classes: [aby::rhi::vulkan::Renderer](classaby_1_1rhi_1_1vulkan_1_1_renderer.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**add\_pass**](#function-add_pass) (std::shared\_ptr&lt; [**RenderPass**](classaby_1_1rhi_1_1_render_pass.md) &gt; render\_pass) = 0<br> |
| virtual void | [**deinit**](#function-deinit) () = 0<br> |
| virtual bool | [**init**](#function-init) (void \* native\_window) = 0<br> |
| virtual bool | [**on\_begin**](#function-on_begin) () = 0<br> |
| virtual bool | [**on\_end**](#function-on_end) () = 0<br> |
| virtual void | [**set\_clear\_color**](#function-set_clear_color) (Color color) = 0<br> |
| virtual  | [**~Renderer**](#function-renderer) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Renderer**](classaby_1_1rhi_1_1_renderer.md) \* | [**create**](#function-create) (ERenderer backend, const [**GraphicsParams**](structaby_1_1rhi_1_1_graphics_params.md) & graphics) <br>_Create a renderer for the backend._  |


























## Public Functions Documentation




### function add\_pass 

```C++
virtual void aby::rhi::Renderer::add_pass (
    std::shared_ptr< RenderPass > render_pass
) = 0
```




<hr>



### function deinit 

```C++
virtual void aby::rhi::Renderer::deinit () = 0
```




<hr>



### function init 

```C++
virtual bool aby::rhi::Renderer::init (
    void * native_window
) = 0
```




<hr>



### function on\_begin 

```C++
virtual bool aby::rhi::Renderer::on_begin () = 0
```




<hr>



### function on\_end 

```C++
virtual bool aby::rhi::Renderer::on_end () = 0
```




<hr>



### function set\_clear\_color 

```C++
virtual void aby::rhi::Renderer::set_clear_color (
    Color color
) = 0
```




<hr>



### function ~Renderer 

```C++
virtual aby::rhi::Renderer::~Renderer () = default
```




<hr>
## Public Static Functions Documentation




### function create 

_Create a renderer for the backend._ 
```C++
static Renderer * aby::rhi::Renderer::create (
    ERenderer backend,
    const GraphicsParams & graphics
) 
```





**Parameters:**


* `backend` a renderer api backend 
* `graphics` graphics parameters 



**Note:**

You are responsible for freeing this memory. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/renderer.hpp`

