## Interfaces 

The library provides multiple interfaces to control the logic of what happens inside the context.
All of them are optional and default interfaces are provided that can be extended. 

There are currently 4 [interfaces](repo:include/aby-rhi/interfaces/interfaces.hpp) that can be override.
They can be set by calling `Context::get().set_interface<YourInterfaceClass>();`

| Interfaces    | Description                                                        | Default            |
| ------------- | ------------------------------------------------------------------ | ------------------ |
| `ILogger`     | Recieves log message with a log level                              | [DefaultLogger](repo:include/aby-rhi/interfaces/default_logger.hpp)      |
| `IAllocator`  | Manages aligned allocations and frees                              | [DefaultAllocator](repo:include/aby-rhi/interfaces/default_allocator.hpp)   |           
| `IFileIO`     | Reads and writes files. As well as managing the CWD and cache dir  | [DefaultFileIO](repo:include/aby-rhi/interfaces/default_fileio.hpp)      |                                   
| `IJobSystem`  | Handles running arbitrary work on multiple threads                 | [DefaultJobSystem](repo:include/aby-rhi/interfaces/default_job_system.hpp)   |            