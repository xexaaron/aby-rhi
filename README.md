# README

## Cloning
```bash
git clone --depth=1 https://github.com/xexaaron/aby-rhi <path>
cd <path>
```
## Building
```bash
cmake -S . -B bin -DCMAKE_BUILD_TYPE=<Debug|Release>
cmake --build bin --config <debug|release>
```
### or
In your CMakeLists.txt file
```cmake
set(ABY_RHI_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(path/to/aby-rhi)
```



## Linking 
The link target on windows msvc must be a static runtime.