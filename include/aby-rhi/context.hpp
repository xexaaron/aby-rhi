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
		void* native_window; // needed for surface creation
		EAntiAliasing aliasing;
	};

	class Context {
	public:
		/**
         * @brief Get the static context instance
        */
		static auto get() -> Context&;
		/**
         * @brief initialize the context. this must be called before anything else relating to the aby::rhi API
         * @param renderer_backend which renderer to create
         * @param window_backend platform specific window backend
         * @param native_window Windows: HWND, ...
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
