

# Struct aby::rhi::DefaultJobQueue



[**ClassList**](annotated.md) **>** [**aby**](namespaceaby.md) **>** [**rhi**](namespaceaby_1_1rhi.md) **>** [**DefaultJobQueue**](structaby_1_1rhi_1_1_default_job_queue.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  ConcurrentQueue&lt; IJobSystem::Job &gt; | [**critical**](#variable-critical)  <br> |
|  std::condition\_variable | [**cv**](#variable-cv)  <br> |
|  ConcurrentQueue&lt; IJobSystem::Job &gt; | [**high**](#variable-high)  <br> |
|  ConcurrentQueue&lt; IJobSystem::Job &gt; | [**low**](#variable-low)  <br> |
|  ConcurrentQueue&lt; IJobSystem::Job &gt; | [**medium**](#variable-medium)  <br> |
|  std::mutex | [**mutex**](#variable-mutex)  <br> |
|  std::atomic\_bool | [**running**](#variable-running)   = `{ true }`<br> |
|  std::thread | [**thread**](#variable-thread)  <br> |












































## Public Attributes Documentation




### variable critical 

```C++
ConcurrentQueue<IJobSystem::Job> aby::rhi::DefaultJobQueue::critical;
```




<hr>



### variable cv 

```C++
std::condition_variable aby::rhi::DefaultJobQueue::cv;
```




<hr>



### variable high 

```C++
ConcurrentQueue<IJobSystem::Job> aby::rhi::DefaultJobQueue::high;
```




<hr>



### variable low 

```C++
ConcurrentQueue<IJobSystem::Job> aby::rhi::DefaultJobQueue::low;
```




<hr>



### variable medium 

```C++
ConcurrentQueue<IJobSystem::Job> aby::rhi::DefaultJobQueue::medium;
```




<hr>



### variable mutex 

```C++
std::mutex aby::rhi::DefaultJobQueue::mutex;
```




<hr>



### variable running 

```C++
std::atomic_bool aby::rhi::DefaultJobQueue::running;
```




<hr>



### variable thread 

```C++
std::thread aby::rhi::DefaultJobQueue::thread;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/aby-rhi/interfaces/default_job_system.hpp`

