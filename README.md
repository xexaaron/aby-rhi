# Abyss Renderer Hardware Interface

## Table of Contents

- [Dependency Requirements](#dependency-requirements)
- [Platforms & Backends](#platforms--backends)
- [Cloning](#cloning)
- [Building](#building)
  - [Options](#options)
  - [CMake](#cmake)
- [Features](#features)
    - [Interfaces](#interfaces)


## Dependency Requirements

| Dependency | Version | Note |
| ---------- | ------- | ---- |
| [![Vulkan](https://img.shields.io/badge/Vulkan-AE0F28?logo=Vulkan&logoColor=fff)](#) | ![Version](https://img.shields.io/badge/version-%3E=_1.4.341-blue) | VulkanSDK for Vulkan Backend
| [![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=fff)](#) | ![Version](https://img.shields.io/badge/version-%3E=_3.14.17-blue) | Shaderc dependency syncing |
| [![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=fff)](#) | ![Version](https://img.shields.io/badge/version-%3E=_3.28.3-blue) | Building the project |

## Platforms & Backends
| Platform | Status | Backend | Status |
| --------- | ------ | ------- | ------ |
| [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#) | ![Platform](https://img.shields.io/badge/platform-Passing-green) | [![Vulkan](https://img.shields.io/badge/Vulkan-AE0F28?logo=Vulkan&logoColor=fff)](#) | ![Backend](https://img.shields.io/badge/backend-Passing-green) |
| [![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#) | ![Platform](https://img.shields.io/badge/platform-Passing-green) | [![Direct3D 12](https://img.shields.io/badge/Direct3D%2012-0078D4?logo=microsoft&logoColor=white)](#) | ![Backend](https://img.shields.io/badge/backend-failing-red) |
| [![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)](#) | ![Platform](https://img.shields.io/badge/platform-Unknown-yellow) | [![Metal](https://img.shields.io/badge/Metal-000000?logo=apple&logoColor=white)](#) | ![Backend](https://img.shields.io/badge/backend-failing-red) |
| | | [![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?logo=opengl&logoColor=white)](#) | ![Backend](https://img.shields.io/badge/backend-failing-red) |

## Cloning

```bash
git clone --depth=1 --recurse-submodules https://github.com/xexaaron/aby-rhi <path>
cd <path>
```

Or as a submodule:

```bash
git submodule add --depth=1 https://github.com/xexaaron/aby-rhi <path>
git submodule update --init --recursive
```

Once one of the two steps above are done, additional setup is required for shaderc:

```bash
cd <path>/vendor/shaderc
py utils/git-sync-deps
```

This will get the proper dependencies for shaderc.

---

## Building

### Options

| Option                     | Description                                                       | Default |
| -------------------------- | ----------------------------------------------------------------- | ------- |
| `ABY_RHI_BUILD_TESTS`      | build aby-rhi tests for the specific platform                     | ![value](https://img.shields.io/badge/value-OFF-red)  |
| `ABY_RHI_ENABLE_LOG_INFO`  | enable info logging                                               | ![value](https://img.shields.io/badge/value-ON-green) |
| `ABY_RHI_ENABLE_LOG_TRACE` | enable trace logging                                              | ![value](https://img.shields.io/badge/value-ON-green) |
| `ABY_RHI_ENABLE_LOG_WARN`  | enable warning logging                                            | ![value](https://img.shields.io/badge/value-ON-green) |
| `ABY_RHI_ENABLE_ASSERT`    | enable assertions. if turned off then only errors will be logged. | ![value](https://img.shields.io/badge/value-ON-green) |
| `ABY_RHI_ENABLE_PROFILING` | enable profiling of initialization scopes                         | ![value](https://img.shields.io/badge/value-OFF-red)  |
| `ABY_RHI_BUILD_DOCS`       | build the mkdocs documentation                                    | ![value](https://img.shields.io/badge/value-OFF-red)  |

### CMake

```bash
cmake -S . -B bin -DCMAKE_BUILD_TYPE=<Debug|Release>
cmake --build bin --config <debug|release>
```

Or, in your CMakeLists.txt file:

```cmake
add_subdirectory(path/to/aby-rhi)

target_link_libraries(${YOUR_PROJECT_NAME} PRIVATE aby-rhi::aby-rhi)

# On MSVC Windows you must ensure the target linking to this project uses a static multithreaded runtime.
set_target_properties(${YOUR_PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
```

## Features

| Feature     | Description                                           | Status   |
| ----------- | ----------------------------------------------------- | -------- |
| `Interfaces`  | Context interfaces that can be overridden             | ![status](https://img.shields.io/badge/status-Complete-green) |
| `Shaders`     | Shader compilation, uniforms, etc...                  | ![status](https://img.shields.io/badge/status-Complete-green) |
| `Textures`    | Texture reading, mipmaps, anisotropy, etc...          | ![status](https://img.shields.io/badge/status-Complete-green) |
| `RenderPass`  | Draw command, uniforms, and per pass shaders          | ![status](https://img.shields.io/badge/status-Complete-green) |

### Interfaces 

The library provides multiple interfaces to control the logic of what happens inside the context.
All of them are optional and default interfaces are provided that can be extended. 

There are currently 4 [interfaces](include/aby-rhi/interfaces/interfaces.hpp) that can be overriden.

| Interfaces    | Description                                                        | Default            |
| ------------- | ------------------------------------------------------------------ | ------------------ |
| `ILogger`     | Recieves log message with a log level                              | [DefaultLogger](include/aby-rhi/interfaces/default_logger.hpp)      |
| `IAllocator`  | Manages aligned allocations and frees                              | [DefaultAllocator](include/aby-rhi/interfaces/default_allocator.hpp)   |           
| `IFileIO`     | Reads and writes files. As well as managing the CWD and cache dir  | [DefaultFileIO](include/aby-rhi/interfaces/default_fileio.hpp)      |                                   
| `IJobSystem`  | Handles running arbitrary work on multiple threads                 | [DefaultJobSystem](include/aby-rhi/interfaces/default_job_system.hpp)   |                      

---

[Scroll to top](#abyss-renderer-hardware-interface)