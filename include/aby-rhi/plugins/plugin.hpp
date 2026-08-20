#pragma once
#include "common.hpp"

namespace aby::rhi {

	enum class EInitTime {
		none, // The default state, should not be handled directly by the interface, this is for making sure plugins arent registered after/during context init.
		pre,  // before anything is initialized, no interfaces, no renderer.
		main, // after the context is initialized, before the render is initialized.
		post, // after everything has been initialized.
		done, // The end state, should not be handled directly by the interface, this is for making sure plugins arent registered after/during context init.
	};

	enum class EDeinitTime {
		pre,  // before anything is deinitialized
		main, // after the renderer is deinitialized, before interfaces/context is deinitialized
		post, // after everything has been deinitialized
	};

	class Plugin {
	public:
		/**
         * @brief Called during initialization
         * @param init_time This function will be called 3 times and you can choose to do work
         *                  at certain points [pre, main, post]
         * @return true if success, false otherwise. (all initialization will return false if this returns false)
         */
		virtual auto on_init(EInitTime init_time) -> bool;
		/**
         * @brief Called after Renderer::on_begin
         */
		virtual auto on_begin() -> void;
		/**
         * @brief Called before Renderer::on_end
         */
		virtual auto on_end() -> void;
		/**
         * @brief Called during deinitialization
         * @param deinit_time This function will be called 3 times and you can choose to do work
         *                    at certain points [pre, main, post]
        */
		virtual auto on_deinit(EDeinitTime deinit_time) -> void;
		/**
         * @brief Called when a resource has finished loading 
        */
		virtual auto on_resource_loaded(ResourceID id, EResource type) -> void;
		/**
         * @brief Called when a resource has been unloaded.
         * @note  its id is now valid to be reused
         */
		virtual auto on_resource_unloaded(ResourceID id, EResource type) -> void;
		/**
         * @brief Called when a resource has failed to load.
         * @return true if ok to continue, false if the rhi should assert.
         */
		virtual auto on_resource_failed(ResourceID id, EResource type) -> bool;
	private:
	};

} // namespace aby::rhi
