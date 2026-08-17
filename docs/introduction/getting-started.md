# Getting Started

Quick guide for cloning, building.

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