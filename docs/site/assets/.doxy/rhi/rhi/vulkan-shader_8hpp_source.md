

# File vulkan-shader.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-shader.hpp**](vulkan-shader_8hpp.md)

[Go to the documentation of this file](vulkan-shader_8hpp.md)


```C++
#pragma once
#include "shader.hpp"

#include <unordered_map>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    class Shader : public aby::rhi::Shader {
    public:
        Shader(EShader type, const std::vector<uint32_t>& data);
        ~Shader();

        auto data() -> std::span<uint32_t>;
        auto bind() -> void override;
        auto destroy() -> void override;
        auto module() -> vk::ShaderModule;
        auto type() const -> EShader override;
    private:
        EShader m_Type;
        vk::ShaderModule m_Module;
        std::vector<uint32_t> m_Data;
    };

} // namespace aby::rhi::vulkan
```


