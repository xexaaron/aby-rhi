

# File context.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**context.hpp**](context_8hpp.md)

[Go to the documentation of this file](context_8hpp.md)


```C++
#pragma once
#include "interfaces/interfaces.hpp"
#include "renderer.hpp"
#include "resource.hpp"
#include "texture.hpp"

#include <concepts>
#include <type_traits>
#include <vector>

namespace aby::rhi {

    struct ContextParams {
        ERenderer renderer_backend;
        EWindow window_backend;
        void* native_window;
        GraphicsParams graphics;
    };

    class Context {
    public:
        static auto get() -> Context&;
        auto init(const ContextParams& params) -> bool;
        auto deinit() -> void;
        auto renderer_backend() const -> ERenderer;
        auto window_backend() const -> EWindow;
        auto renderer() -> Renderer*;
        auto logger() -> ILogger*;
        auto allocator() -> IAllocator*;
        auto file_io() -> IFileIO*;
        auto job_sys() -> IJobSystem*;
        auto textures() -> ResourceContainer<Texture, EResource::texture>&;
        auto shaders() -> ResourceContainer<Shader, EResource::shader>&;

        template <typename T>
        requires(std::derived_from<T, IInterface>)
        auto set_interface() -> void {
            if constexpr (std::is_base_of_v<ILogger, T>) {
                m_Logger = new T();
            } else if constexpr (std::is_base_of_v<IAllocator, T>) {
                m_Allocator = new T();
            } else if constexpr (std::is_base_of_v<IFileIO, T>) {
                m_FileIO = new T();
            } else if constexpr (std::is_base_of_v<IJobSystem, T>) {
                m_JobSystem = new T();
            } else {
                static_assert(std::false_type::value, "unimplemented interface type");
            }
        }
    private:
        ILogger* m_Logger;
        IAllocator* m_Allocator;
        IFileIO* m_FileIO;
        IJobSystem* m_JobSystem;
        Renderer* m_Renderer;
        ContextParams m_Params;
        ResourceContainer<Texture, EResource::texture> m_Textures;
        ResourceContainer<Shader, EResource::shader> m_Shaders;
    };

} // namespace aby::rhi
```


