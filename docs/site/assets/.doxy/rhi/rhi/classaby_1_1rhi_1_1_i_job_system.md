

# Class aby::rhi::IJobSystem



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**IJobSystem**](classaby_1_1rhi_1_1_i_job_system.md)








Inherits the following classes: [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)


Inherited by the following classes: [aby::rhi::DefaultJobSystem](classaby_1_1rhi_1_1_default_job_system.md)












## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void()&gt; | [**Job**](#typedef-job)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**add\_job**](#function-add_job) (EJobPriority priority, Job && job) = 0<br>_Add a job to be ran asynchronously._  |
| virtual void | [**destroy**](#function-destroy) () = 0<br>_Should cleanup any resources the JobSystem uses and finish all jobs currently running. The jobs must finish because they could be jobs such as caching data._  |
| virtual std::string\_view | [**name**](#function-name) () override<br> |
| virtual size\_t | [**thread\_count**](#function-thread_count) () = 0<br>_Get the number of threads that jobs can be run on._  |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |






















































## Public Types Documentation




### typedef Job 

```C++
using aby::rhi::IJobSystem::Job =  std::function<void()>;
```




<hr>
## Public Functions Documentation




### function add\_job 

_Add a job to be ran asynchronously._ 
```C++
virtual void aby::rhi::IJobSystem::add_job (
    EJobPriority priority,
    Job && job
) = 0
```





**Parameters:**


* `priority` The order in which the jobs should be completed. 




        

<hr>



### function destroy 

_Should cleanup any resources the JobSystem uses and finish all jobs currently running. The jobs must finish because they could be jobs such as caching data._ 
```C++
virtual void aby::rhi::IJobSystem::destroy () = 0
```




<hr>



### function name 

```C++
virtual std::string_view aby::rhi::IJobSystem::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>



### function thread\_count 

_Get the number of threads that jobs can be run on._ 
```C++
virtual size_t aby::rhi::IJobSystem::thread_count () = 0
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/interfaces.hpp`

