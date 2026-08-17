

# Class aby::rhi::ResourcePtr

**template &lt;typename T, EResource ResourceType&gt;**



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**ResourcePtr**](classaby_1_1rhi_1_1_resource_ptr.md)



[_**ResourcePtr**_](classaby_1_1rhi_1_1_resource_ptr.md) _class that abstracts the synchronization mechanisms from the user._[More...](#detailed-description)

* `#include <resource.hpp>`



Inherits the following classes: [aby::rhi::Resource](classaby_1_1rhi_1_1_resource.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ResourcePtr**](#function-resourceptr-13) () <br> |
|   | [**ResourcePtr**](#function-resourceptr-23) (std::nullptr\_t) <br> |
|   | [**ResourcePtr**](#function-resourceptr-33) (ResourceID id, [**ResourceContainer**](classaby_1_1rhi_1_1_resource_container.md)&lt; T, ResourceType &gt; \* container) <br> |
|  T \* | [**get**](#function-get) () <br>_Get the underlying resource. waits for the resource to be loaded if it is not loaded._  |
|   | [**operator bool**](#function-operator-bool) () const<br>_Checks if the resource id is invalid._  |
|  T & | [**operator\***](#function-operator) () <br>_Get the underlying resource. waits for the resource to be loaded if it is not loaded._  |
|  T \* | [**operator-&gt;**](#function-operator-) () <br>_Get the underlying resource. waits for the resource to be loaded if it is not loaded._  |


## Public Functions inherited from aby::rhi::Resource

See [aby::rhi::Resource](classaby_1_1rhi_1_1_resource.md)

| Type | Name |
| ---: | :--- |
|   | [**Resource**](classaby_1_1rhi_1_1_resource.md#function-resource) (EResource type=EResource::none, ResourceID id=ResourceID::invalid) <br>[_**Resource**_](classaby_1_1rhi_1_1_resource.md) _constructor. Users should not construct Resources themselves and use the appropriate resource Create method._ |
|  ResourceID | [**id**](classaby_1_1rhi_1_1_resource.md#function-id) () const<br> |
|   | [**operator ResourceID**](classaby_1_1rhi_1_1_resource.md#function-operator-resourceid) () const<br> |
|   | [**operator bool**](classaby_1_1rhi_1_1_resource.md#function-operator-bool) () const<br> |
|   | [**pair&lt; EResource, ResourceID &gt;**](classaby_1_1rhi_1_1_resource.md#function-pair-eresource-resourceid) () const<br> |
|  bool | [**operator==**](classaby_1_1rhi_1_1_resource.md#function-operator) ([**Resource**](classaby_1_1rhi_1_1_resource.md) other) const<br> |
|  EResource | [**type**](classaby_1_1rhi_1_1_resource.md#function-type) () const<br> |






















































## Detailed Description




**Template parameters:**


* `T` the underlying resource type 
* `ResourceType` the resource type of the container that owns the underlying resource data 




    
## Public Functions Documentation




### function ResourcePtr [1/3]

```C++
aby::rhi::ResourcePtr::ResourcePtr () 
```




<hr>



### function ResourcePtr [2/3]

```C++
aby::rhi::ResourcePtr::ResourcePtr (
    std::nullptr_t
) 
```




<hr>



### function ResourcePtr [3/3]

```C++
aby::rhi::ResourcePtr::ResourcePtr (
    ResourceID id,
    ResourceContainer < T, ResourceType > * container
) 
```




<hr>



### function get 

_Get the underlying resource. waits for the resource to be loaded if it is not loaded._ 
```C++
T * aby::rhi::ResourcePtr::get () 
```





**Returns:**

If the resource load failed nullptr, otherwise the resource data. 





        

<hr>



### function operator bool 

_Checks if the resource id is invalid._ 
```C++
explicit aby::rhi::ResourcePtr::operator bool () const
```




<hr>



### function operator\* 

_Get the underlying resource. waits for the resource to be loaded if it is not loaded._ 
```C++
T & aby::rhi::ResourcePtr::operator* () 
```





**Returns:**

If the resource load failed assert, otherwise the resource data. 





        

<hr>



### function operator-&gt; 

_Get the underlying resource. waits for the resource to be loaded if it is not loaded._ 
```C++
T * aby::rhi::ResourcePtr::operator-> () 
```





**Returns:**

If the resource load failed nullptr, otherwise the resource data. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/resource.hpp`

