

# Class aby::rhi::DefaultJobSystem



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**DefaultJobSystem**](classaby_1_1rhi_1_1_default_job_system.md)








Inherits the following classes: [aby::rhi::IJobSystem](classaby_1_1rhi_1_1_i_job_system.md)
















## Public Types inherited from aby::rhi::IJobSystem

See [aby::rhi::IJobSystem](classaby_1_1rhi_1_1_i_job_system.md)

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void()&gt; | [**Job**](classaby_1_1rhi_1_1_i_job_system.md#typedef-job)  <br> |


























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DefaultJobSystem**](#function-defaultjobsystem) () <br> |
| virtual void | [**add\_job**](#function-add_job) (EJobPriority priority, Job && job) override<br>_Add a job to be ran asynchronously._  |
| virtual void | [**destroy**](#function-destroy) () override<br>_Should cleanup any resources the JobSystem uses and finish all jobs currently running. The jobs must finish because they could be jobs such as caching data._  |
| virtual std::string\_view | [**name**](#function-name) () override<br> |
| virtual size\_t | [**thread\_count**](#function-thread_count) () override<br>_Get the number of threads that jobs can be run on._  |
|   | [**~DefaultJobSystem**](#function-defaultjobsystem) () <br> |


## Public Functions inherited from aby::rhi::IJobSystem

See [aby::rhi::IJobSystem](classaby_1_1rhi_1_1_i_job_system.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**add\_job**](classaby_1_1rhi_1_1_i_job_system.md#function-add_job) (EJobPriority priority, Job && job) = 0<br>_Add a job to be ran asynchronously._  |
| virtual void | [**destroy**](classaby_1_1rhi_1_1_i_job_system.md#function-destroy) () = 0<br>_Should cleanup any resources the JobSystem uses and finish all jobs currently running. The jobs must finish because they could be jobs such as caching data._  |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_job_system.md#function-name) () override<br> |
| virtual size\_t | [**thread\_count**](classaby_1_1rhi_1_1_i_job_system.md#function-thread_count) () = 0<br>_Get the number of threads that jobs can be run on._  |


## Public Functions inherited from aby::rhi::IInterface

See [aby::rhi::IInterface](classaby_1_1rhi_1_1_i_interface.md)

| Type | Name |
| ---: | :--- |
| virtual std::string\_view | [**name**](classaby_1_1rhi_1_1_i_interface.md#function-name) () = 0<br> |
| virtual  | [**~IInterface**](classaby_1_1rhi_1_1_i_interface.md#function-iinterface) () = default<br> |
















































































## Public Functions Documentation




### function DefaultJobSystem 

```C++
aby::rhi::DefaultJobSystem::DefaultJobSystem () 
```




<hr>



### function add\_job 

_Add a job to be ran asynchronously._ 
```C++
virtual void aby::rhi::DefaultJobSystem::add_job (
    EJobPriority priority,
    Job && job
) override
```





**Parameters:**


* `priority` The order in which the jobs should be completed. 




        
Implements [*aby::rhi::IJobSystem::add\_job*](classaby_1_1rhi_1_1_i_job_system.md#function-add_job)


<hr>



### function destroy 

_Should cleanup any resources the JobSystem uses and finish all jobs currently running. The jobs must finish because they could be jobs such as caching data._ 
```C++
virtual void aby::rhi::DefaultJobSystem::destroy () override
```



Implements [*aby::rhi::IJobSystem::destroy*](classaby_1_1rhi_1_1_i_job_system.md#function-destroy)


<hr>



### function name 

```C++
virtual std::string_view aby::rhi::DefaultJobSystem::name () override
```



Implements [*aby::rhi::IInterface::name*](classaby_1_1rhi_1_1_i_interface.md#function-name)


<hr>



### function thread\_count 

_Get the number of threads that jobs can be run on._ 
```C++
virtual size_t aby::rhi::DefaultJobSystem::thread_count () override
```



Implements [*aby::rhi::IJobSystem::thread\_count*](classaby_1_1rhi_1_1_i_job_system.md#function-thread_count)


<hr>



### function ~DefaultJobSystem 

```C++
aby::rhi::DefaultJobSystem::~DefaultJobSystem () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/default_job_system.hpp`

