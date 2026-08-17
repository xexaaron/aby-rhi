

# Class aby::rhi::Resource



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**Resource**](classaby_1_1rhi_1_1_resource.md)



_A resource is a handle to the resource data contained with the_ [_**Context**_](classaby_1_1rhi_1_1_context.md) _. This is used for loading data asynchronously while still being able to give out valid usable IDs._

* `#include <resource.hpp>`





Inherited by the following classes: [aby::rhi::ResourcePtr](classaby_1_1rhi_1_1_resource_ptr.md),  [aby::rhi::ResourcePtr](classaby_1_1rhi_1_1_resource_ptr.md),  [aby::rhi::ResourcePtr](classaby_1_1rhi_1_1_resource_ptr.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Resource**](#function-resource) (EResource type=EResource::none, ResourceID id=ResourceID::invalid) <br>[_**Resource**_](classaby_1_1rhi_1_1_resource.md) _constructor. Users should not construct Resources themselves and use the appropriate resource Create method._ |
|  ResourceID | [**id**](#function-id) () const<br> |
|   | [**operator ResourceID**](#function-operator-resourceid) () const<br> |
|   | [**operator bool**](#function-operator-bool) () const<br> |
|   | [**pair&lt; EResource, ResourceID &gt;**](#function-pair-eresource-resourceid) () const<br> |
|  bool | [**operator==**](#function-operator) ([**Resource**](classaby_1_1rhi_1_1_resource.md) other) const<br> |
|  EResource | [**type**](#function-type) () const<br> |




























## Public Functions Documentation




### function Resource 

[_**Resource**_](classaby_1_1rhi_1_1_resource.md) _constructor. Users should not construct Resources themselves and use the appropriate resource Create method._
```C++
aby::rhi::Resource::Resource (
    EResource type=EResource::none,
    ResourceID id=ResourceID::invalid
) 
```





**Parameters:**


* `type` The type of resource 
* `id` An id given by the [**Context**](classaby_1_1rhi_1_1_context.md). 




        

<hr>



### function id 

```C++
ResourceID aby::rhi::Resource::id () const
```




<hr>



### function operator ResourceID 

```C++
aby::rhi::Resource::operator ResourceID () const
```




<hr>



### function operator bool 

```C++
explicit aby::rhi::Resource::operator bool () const
```




<hr>



### function pair&lt; EResource, ResourceID &gt; 

```C++
aby::rhi::Resource::pair< EResource, ResourceID > () const
```




<hr>



### function operator== 

```C++
bool aby::rhi::Resource::operator== (
    Resource other
) const
```




<hr>



### function type 

```C++
EResource aby::rhi::Resource::type () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/resource.hpp`

