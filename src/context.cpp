#include "context.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "interfaces/default_interfaces.hpp"
#include <assert.h>

namespace aby::rhi {


    auto Context::get() -> Context& {
        static Context context;
        return context;
    }  

    auto Context::init(ERenderer renderer_backend, EWindow window_backend, void* native_window) -> bool {
        if (!m_Logger)    set_interface<DefaultLogger>();
        if (!m_Allocator) set_interface<DefaultAllocator>();
        if (!m_FileIO)    set_interface<DefaultFileIO>();
        
        m_RendererBackend = renderer_backend;
        m_WinBackend      = window_backend;
        switch (renderer_backend) {
            case ERenderer::vulkan:
                m_Renderer = new vulkan::Renderer();
                if (!m_Renderer->init(native_window)) {
                    return false;
                }
                break;
            default:
                return false;
        }

        return true;
    }


    auto Context::deinit() -> void {
        delete m_Renderer;
        delete m_Logger;
        delete m_Allocator;
        delete m_FileIO;
    }

    auto Context::renderer_backend() const -> ERenderer {
        return m_RendererBackend;
    }

    auto Context::window_backend() const -> EWindow {
        return m_WinBackend;
    }

    auto Context::renderer() -> IRenderer* {
        return m_Renderer;
    }


    auto Context::logger() -> ILogger* {
        return m_Logger;
    }

    auto Context::allocator() -> IAllocator* {
        return m_Allocator;
    }


    auto Context::file_io() -> IFileIO* {
        return m_FileIO;
    }


}