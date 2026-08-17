

# Class aby::rhi::ResourceContainer

**template &lt;typename T, EResource ResourceType&gt;**



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md)



_A resource container that owns the resources and performs cleanup._ [More...](#detailed-description)

* `#include <resource.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**add**](#function-add) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource, T \* obj) <br>_Add an already constructed resource to the container._  |
|  void | [**emplace**](#function-emplace) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource, Args &&... args) <br>_Construct a resource directly into the container._  |
|  void | [**fail**](#function-fail) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource) <br>_Notify the container that resource loading failed._  |
|  bool | [**is\_loaded**](#function-is_loaded) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource) <br>_Check if a resource has finished loading._  |
|  T \* | [**operator[]**](#function-operator) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource) <br>_Access the resource data._  |
|  void | [**remove**](#function-remove) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource) <br>_Remove a resource from the container and free its ID._  |
|  [**Resource**](classaby_1_1rhi_1_1_resource.md) | [**reserve**](#function-reserve) () <br>_Reserve a resource slot in the container. This resource can be passed around before the creation of the actual data._  |
|  EResourceState | [**state**](#function-state) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource) <br>_Get the current state of a resource._  |
|  bool | [**wait\_for**](#function-wait_for) ([**Resource**](classaby_1_1rhi_1_1_resource.md) resource) <br>_Wait until a resource finishes loading._  |
|   | [**~ResourceContainer**](#function-resourcecontainer) () <br> |




























## Detailed Description




**Template parameters:**


* `T` the type of resource. 
* `ResourceType` The type of resource that is owned. Used for runtime checking and resource creation to ensure that resources owned by another container arent accessed using this container. 



**Warning:**

The container does not do bounds checking itself. 





    
## Public Functions Documentation




### function add 

_Add an already constructed resource to the container._ 
```C++
void aby::rhi::ResourceContainer::add (
    Resource resource,
    T * obj
) 
```





**Parameters:**


* `resource` [**Resource**](classaby_1_1rhi_1_1_resource.md) reserved via [**ResourceContainer::reserve**](classaby_1_1rhi_1_1_resource_container.md#function-reserve). 
* `obj` Constructed object. 




        

<hr>



### function emplace 

_Construct a resource directly into the container._ 
```C++
template<typename... Args>
void aby::rhi::ResourceContainer::emplace (
    Resource resource,
    Args &&... args
) 
```





**Parameters:**


* `resource` [**Resource**](classaby_1_1rhi_1_1_resource.md) reserved via [**ResourceContainer::reserve**](classaby_1_1rhi_1_1_resource_container.md#function-reserve). 
* `args` Constructor arguments. 




        

<hr>



### function fail 

_Notify the container that resource loading failed._ 
```C++
void aby::rhi::ResourceContainer::fail (
    Resource resource
) 
```





**Parameters:**


* `resource` [**Resource**](classaby_1_1rhi_1_1_resource.md) reserved via [**ResourceContainer::reserve**](classaby_1_1rhi_1_1_resource_container.md#function-reserve). 




        

<hr>



### function is\_loaded 

_Check if a resource has finished loading._ 
```C++
bool aby::rhi::ResourceContainer::is_loaded (
    Resource resource
) 
```




<hr>



### function operator[] 

_Access the resource data._ 
```C++
T * aby::rhi::ResourceContainer::operator[] (
    Resource resource
) 
```





**Warning:**

[**Resource**](classaby_1_1rhi_1_1_resource.md) must be in the loaded state. 





        

<hr>



### function remove 

_Remove a resource from the container and free its ID._ 
```C++
void aby::rhi::ResourceContainer::remove (
    Resource resource
) 
```





**Parameters:**


* `resource` [**Resource**](classaby_1_1rhi_1_1_resource.md) handle. 




        

<hr>



### function reserve 

_Reserve a resource slot in the container. This resource can be passed around before the creation of the actual data._ 
```C++
Resource aby::rhi::ResourceContainer::reserve () 
```





**Returns:**

A resource handle that can later be passed to add/emplace. 





        

<hr>



### function state 

_Get the current state of a resource._ 
```C++
EResourceState aby::rhi::ResourceContainer::state (
    Resource resource
) 
```




<hr>



### function wait\_for 

_Wait until a resource finishes loading._ 
```C++
bool aby::rhi::ResourceContainer::wait_for (
    Resource resource
) 
```





**Returns:**

False if loading failed or resource is invalid, true if loaded. 





        

<hr>



### function ~ResourceContainer 

```C++
aby::rhi::ResourceContainer::~ResourceContainer () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/resource.hpp`

