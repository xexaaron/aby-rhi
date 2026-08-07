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
		/// @brief The renderer backend to create
		ERenderer renderer_backend;
		/// @brief The window backend to use (provided because their are multiple windowing backends on linux)
		EWindow window_backend;
		/**
		 * @brief The native window handle(s)
		 * @param Win32   HWND
		 * @param X11 	  std::pair<Display*, Window>
		 * @param XCB     std::pair<xcb_connection_t*, xcb_window_t>
		 * @param Wayland std::pair<wl_display*, wl_surface*>
		 * @param Metal   CAMetalLayer*
		*/
		void* native_window;
		/// @brief The graphics configuraton to pass to the renderer.
		GraphicsParams graphics;
	};

	class Context {
	public:
		/**
         * @brief Get the static context instance
        */
		static auto get() -> Context&;
		/**
         * @brief initialize the context. this must be called before anything else relating to the aby::rhi API
         * @param context_params context parameters structure
         * @return true on success otherwise false
         */
		auto init(const ContextParams& params) -> bool;
		/**
         * @brief Must be called at the end of the application to release all resources.
         */
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

		/**
         * @brief Set the interface type. Call this before calling Context::init.
         * @tparam ILogger Logging interface [optional]
         * @tparam IAllocator Allocator interface [optional]
         * @tparam IFileIO File io interface [optional]
		 * @tparam IJobSystem Job system interface [optional]
         */
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
