#pragma once
#include "interfaces.hpp"
#include "common.hpp"
#include "renderer.hpp"
#include <type_traits>
#include <vector> 
#include <concepts>

namespace aby::rhi {

    class Context {
    public:
        static auto get() -> Context&;  
        auto init(ERenderer renderer_backend, EWindow window_backend, void* native_window) -> bool;
        auto deinit() -> void; 

        auto renderer_backend() const -> ERenderer;
        auto window_backend() const -> EWindow;
        auto renderer() -> IRenderer*;
        auto logger() -> ILogger*;
        auto allocator() -> IAllocator*;

        /**
         * @brief Set the interface type. Call this before calling Context::init.
         * @tparam ILogger Logging interface [required]
         * @tparam IAllocator Allocator interface [optional]
         */
        template <typename T> requires(std::derived_from<T, IInterface>) 
        auto set_interface() -> void {
            if constexpr (std::is_base_of_v<ILogger, T>) {
                m_Logger = new T();
            } 
            else if constexpr (std::is_base_of_v<IAllocator, T>) {
                m_Allocator = new T();
            }
            else {
                static_assert(std::false_type::value, "unimplemented interface type");
            }
        }
    private:
        ILogger*    m_Logger;
        IAllocator* m_Allocator;
        IRenderer*  m_Renderer;
        ERenderer   m_RendererBackend;
        EWindow     m_WinBackend;
    };


}