# Abyss Renderer Hardware Interface


## Table of Contents

- [Cloning](#cloning)
- [Building](#building)
  - [Options](#options)
  - [CMake](#cmake)
- [Quick Start](#quick-start)
- [Features](#features)
    - [Interfaces](#interfaces)
- [Textures](#textures)

---

## Platforms
| Platform  | Status |
| --------- | ------ |
| [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#) |  ![Platform](https://img.shields.io/badge/platform-Passing-green)
| [![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#) | ![Platform](https://img.shields.io/badge/platform-Unknown-yellow)
| [![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)](#) | ![Platform](https://img.shields.io/badge/platform-Unknown-yellow)

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

# Quick Start

C++ Quick start guide

If you want a more in depth example into the test directory at the platform specific tests.

```cpp
// Context is a singleton. Retrieve the instance instead of creating one.
auto& ctx = Context::get();

// Fill out the context parameters with:
// - the renderer backend
// - the window backend/platform
// - the native window handle
// - any additional features
ContextParams context_params{...};

// Initialize the context before doing any other work.
if (!ctx.init(context_params))
    return 1;

// If using the default file IO interface, set the working directory
// and cache directory. These paths are used for loading resources such
// as textures, shaders, and other files.
ctx.file_io()->set_cwd(fs::path(argv[0]).parent_path());
ctx.file_io()->set_cache_dir(ctx.file_io()->cwd() / "cache");


// Create a render pass.
//
// Add shaders, configure uniforms, and define the vertex layout.
// The vertex description builder returns back to the render pass builder.
auto rpsb = RenderPassBuilder::create();

auto pass = rpsb->add_shader("vertex_shader.vert")
    .add_uniform(...)
    .vertex_description_builder()
        .add_input<&Vertex::pos>(EFormat::rgb_f32)
        .add_inputs<&Vertex::uv, &Vertex::color>(EFormat::rg_f32, EFormat::rgba_f32)
        .build()
    ->add_shader("fragment_shader.frag")
    .use_all_defaults()
    .set_cull_mode(ECullMode::front, EFrontFace::counter_clockwise)
    .disable_blending()
    .disable_depthtest()
    .build();

// Register the render pass with the renderer.
ctx.renderer().add_pass(pass);


// Create vertex and index buffers.
//
// This creates a vertex buffer capable of holding 100 vertices.
auto vbuff = VertexBuffer::create(100, sizeof(Vertex));

// This creates an index buffer capable of holding 60 uint32_t indices.
auto ibuff = IndexBuffer::create(60);

// Push individual vertices and indices into the buffers.
//
// sizeof(Vertex) must match the size passed when creating the vertex buffer.
Vertex v{...};

vbuff->push(&v);
ibuff->push(uint32_t(1));


// Create a draw command from the vertex and index buffers.
DrawCmd cmd(vbuff, ibuff);

// Main render loop.
auto* renderer = ctx.renderer();

while (running) {
    ///// event loop logic

    if (!running)
        break;

    if (!renderer->on_begin())
        continue;

    ///// update logic

    // Update dynamic uniforms.
    pass->set_uniforms(...);

    // Submit draw commands.
    pass->submit(cmd);

    renderer->on_end();
}


// Cleanup.
ctx.deinit();
```

# Features

| Feature     | Description                                           | Status   |
| ----------- | ----------------------------------------------------- | -------- |
| Interfaces  | Context interfaces that can be overridden             | ![status](https://img.shields.io/badge/status-Complete-green)_ |
| Shaders     | Shader compilation, uniforms, etc...                  | ![Backend](https://img.shields.io/badge/backend-Vulkan-green) ![Backend](https://img.shields.io/badge/backend-D3D12-red) ![Backend](https://img.shields.io/badge/backend-Metal-red)   |
| Textures    | Texture reading, mipmaps, anisotropy, etc...          | ![Backend](https://img.shields.io/badge/backend-Vulkan-green) ![Backend](https://img.shields.io/badge/backend-D3D12-red) ![Backend](https://img.shields.io/badge/backend-Metal-red)   |
| RenderPass  | Draw command, uniforms, and per pass shaders          | ![Backend](https://img.shields.io/badge/backend-Vulkan-green) ![Backend](https://img.shields.io/badge/backend-D3D12-red) ![Backend](https://img.shields.io/badge/backend-Metal-red)   |
| Backends    | Vulkan, D3D12, Metal                                  | ![Backend](https://img.shields.io/badge/backend-Vulkan-green) ![Backend](https://img.shields.io/badge/backend-D3D12-red) ![Backend](https://img.shields.io/badge/backend-Metal-red)   |


## Interfaces 

The library provides multiple interfaces to control the logic of what happens inside the context.
All of them are optional and default interfaces are provided that can be extended. 

There are currently 4 [interfaces](include/aby-rhi/interfaces/interfaces.hpp) that can be override.
They can be set by calling `Context::get().set_interface<YourInterfaceClass>();`

| Interfaces    | Description                                                        | Default            |
| ------------- | ------------------------------------------------------------------ | ------------------ |
| `ILogger`     | Recieves log message with a log level                              | [DefaultLogger](include/aby-rhi/interfaces/default_logger.hpp)      |
| `IAllocator`  | Manages aligned allocations and frees                              | [DefaultAllocator](include/aby-rhi/interfaces/default_allocator.hpp)   |           
| `IFileIO`     | Reads and writes files. As well as managing the CWD and cache dir  | [DefaultFileIO](include/aby-rhi/interfaces/default_fileio.hpp)      |                                   
| `IJobSystem`  | Handles running arbitrary work on multiple threads                 | [DefaultJobSystem](include/aby-rhi/interfaces/default_job_system.hpp)   |                      


# Textures

