# README

## Cloning & Building
```bash
git clone --depth=1 https://github.com/xexaaron/aby-rhi <path>
cd <path>

cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug
cmake --build bin --config <debug|release>
```
## Linking 
The link target on windows msvc must be a static runtime.