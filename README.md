# README

## Cloning
```bash
git clone --depth=1 --recurse-submodules https://github.com/xexaaron/aby-rhi <path>
cd <path>
# or as a submodule
git submodule add --depth=1 https://github.com/xexaaron/aby-rhi <path>
git submodule update --init --recursive
# once one of the two above steps are done some more setup is required for shaderc
cd <path>/vendor/shaderc
py utils/git-sync-deps
# this will get the proper dependencies for shaderc 
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