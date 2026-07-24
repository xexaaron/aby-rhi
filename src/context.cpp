#include "context.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include <assert.h>

namespace aby::rhi {

    class DefaultAllocator : public IAllocator {
    public:
        auto name() -> std::string_view override { 
            return "IAllocator";
        };

        auto alloc(size_t bytes, size_t alignment, EAllocation type) -> void* {
            #ifdef _MSC_VER
            #   ifndef _NDEBUG
                    return _aligned_malloc_dbg(bytes, alignment, __FILE__, __LINE__);
            #   else
                    return _aligned_malloc(bytes, alignment);
            #   endif
            #else
                    return std::aligned_alloc(alignment, bytes);
            #endif
        };
        
        auto realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* {
            #ifdef _MSC_VER
            #   ifndef _NDEBUG
                    return _aligned_realloc_dbg(ptr, bytes, alignment, __FILE__, __LINE__);
            #   else
                    return _aligned_realloc(ptr, bytes, alignment);
            #   endif
            #else
                    return std::aligned_realloc(ptr, alignment, bytes);
            #endif
        };
        
        auto free(void* ptr) -> void override {
            #ifdef _MSC_VER
            #   ifndef _NDEBUG
                    _aligned_free_dbg(ptr);
            #   else
                    _aligned_free(ptr);
            #   endif
            #else
                    std::free(ptr);
            #endif
        };

        auto usage() -> std::unordered_map<EAllocation, size_t> override {
            return m_Usage;
        }
    private:
        std::unordered_map<EAllocation, size_t> m_Usage;
    };

    auto Context::get() -> Context& {
        static Context context;
        return context;
    }  

    auto Context::init(ERenderer renderer_backend, EWindow window_backend, void* native_window) -> bool {
        assert(m_Logger && "Context::set_interface<...>() was not called with a valid logger subclass");        
        if (!m_Allocator) {
            set_interface<DefaultAllocator>();
        }
    
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


}