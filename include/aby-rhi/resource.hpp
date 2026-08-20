#pragma once
#include "common.hpp"
#include "plugins/plugin.hpp"

#include <deque>
#include <mutex>
#include <print>
#include <queue>
#include <span>
#include <vector>

namespace aby::rhi {

	/**
     * @brief A resource is a handle to the resource data contained with the Context.
     *        This is used for loading data asynchronously while still being able to give out
     *        valid usable IDs. 
    */
	class Resource {
	public:
		/**
         * @brief Resource constructor. Users should not construct Resources themselves and 
         *        use the appropriate resource Create method.
         * @param type The type of resource
         * @param id An id given by the Context. 
         */
		Resource(EResource type = EResource::none, ResourceID id = ResourceID::invalid);

		auto type() const -> EResource;
		auto id() const -> ResourceID;

		operator ResourceID() const;
		operator std::pair<EResource, ResourceID>() const;

		explicit operator bool() const;
		auto operator==(Resource other) const -> bool;
	private:
		EResource m_Type;
		ResourceID m_ID;
	};

	/**
	 * @brief A resource container that owns the resources and performs cleanup
	 * @tparam T the type of resource.
	 * @tparam ResourceType The type of resource that is owned. Used for runtime checking and resource creation
	 *                       to ensure that resources owned by another container arent accessed using this container.
	 * @warning The container does not do bounds checking itself.
	 */
	template <typename T, EResource ResourceType>
	class ResourceContainer {
	public:
		~ResourceContainer();

		/**
		 * @brief Clear the container and delete all resources, destroying them in the process. 
		 */
		auto clear() -> void;

		/**
		 * @brief Reserve a resource slot in the container. This resource can be passed around before the creation
		 *        of the actual data.
		 * @return A resource handle that can later be passed to add/emplace.
		 */
		auto reserve() -> Resource;

		auto set_plugins(std::vector<Plugin*>& plugins) -> void;

		/**
		 * @brief Add an already constructed resource to the container.
		 * @param resource Resource reserved via ResourceContainer::reserve.
		 * @param obj Constructed object.
		 */
		auto add(Resource resource, T* obj) -> void;

		/**
		 * @brief Construct a resource directly into the container.
		 * @param resource Resource reserved via ResourceContainer::reserve.
		 * @param args Constructor arguments.
		 */
		template <typename... Args>
		requires(std::is_constructible_v<T, Args...>)
		auto emplace(Resource resource, Args&&... args) -> void;

		/**
		 * @brief Remove a resource from the container and free its ID.
		 * @param resource Resource handle.
		 */
		auto remove(Resource resource) -> void;

		/**
		 * @brief Notify the container that resource loading failed.
		 * @param resource Resource reserved via ResourceContainer::reserve.
		 */
		auto fail(Resource resource) -> void;

		/**
		 * @brief Wait until a resource finishes loading.
		 * @return False if loading failed or resource is invalid, true if loaded.
		 */
		auto wait_for(Resource resource) -> bool;

		/**
		 * @brief Check if a resource has finished loading.
		 */
		auto is_loaded(Resource resource) -> bool;

		/**
		 * @brief Get the current state of a resource.
		 */
		auto state(Resource resource) -> EResourceState;

		/**
		 * @brief Access the resource data.
		 * @warning Resource must be in the loaded state.
		 */
		auto operator[](Resource resource) -> T*;
	private:
		std::vector<T*> m_Resources;
		std::queue<ResourceID> m_FreeIDs;
		std::deque<std::atomic<EResourceState>> m_ResourceStates;
		std::span<Plugin*> m_Plugins;
		std::mutex m_IndexMutex;
	};

	/**
	 * @brief ResourcePtr class that abstracts the synchronization mechanisms from the user
	 * @tparam T the underlying resource type
	 * @tparam ResourceType the resource type of the container that owns the underlying resource data
	 */
	template <typename T, EResource ResourceType>
	class ResourcePtr : public Resource {
	public:
		ResourcePtr();
		ResourcePtr(std::nullptr_t);
		ResourcePtr(ResourceID id, ResourceContainer<T, ResourceType>* container);

		/**
		 * @brief Get the underlying resource. waits for the resource to be loaded if it is not loaded.
		 * @return If the resource load failed nullptr, otherwise the resource data.
		 */
		auto operator->() -> T*;
		/**
		 * @brief Get the underlying resource. waits for the resource to be loaded if it is not loaded.
		 * @return If the resource load failed assert, otherwise the resource data.
		*/
		auto operator*() -> T&;
		/**
		 * @brief Get the underlying resource. waits for the resource to be loaded if it is not loaded.
		 * @return If the resource load failed nullptr, otherwise the resource data.
		*/
		auto get() -> T*;

		/**
		 * @brief Checks if the resource id is invalid
		*/
		explicit operator bool() const;
	private:
		mutable T* m_Cached                             = nullptr;
		ResourceContainer<T, ResourceType>* m_Container = nullptr;
	};

	template <typename T, EResource ResourceType>
	auto create_resource(Resource resource, ResourceContainer<T, ResourceType>& container) -> ResourcePtr<T, ResourceType> {
		if (resource.type() != ResourceType)
			return {};
		return ResourcePtr<T, ResourceType>(resource.id(), &container);
	}

} // namespace aby::rhi

namespace aby::rhi {

	template <typename T, EResource ResourceType>
	ResourceContainer<T, ResourceType>::~ResourceContainer() {
		clear();
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::clear() -> void {
		std::lock_guard lock_guard(m_IndexMutex);

		for (auto* resource : m_Resources) {
			delete resource;
		}

		m_Resources.clear();
		m_ResourceStates.clear();

		while (!m_FreeIDs.empty()) {
			m_FreeIDs.pop();
		}
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::set_plugins(std::vector<Plugin*>& plugins) -> void {
		m_Plugins = plugins;
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::reserve() -> Resource {
		std::lock_guard lock(m_IndexMutex);

		ResourceID id = ResourceID::invalid;

		if (!m_FreeIDs.empty()) {
			id = m_FreeIDs.front();
			m_FreeIDs.pop();
			m_ResourceStates[static_cast<size_t>(id)].store(EResourceState::loading);
		} else {
			id = static_cast<ResourceID>(m_Resources.size());
			m_Resources.push_back(nullptr);
			m_ResourceStates.emplace_back(EResourceState::loading);
		}

		return Resource(ResourceType, id);
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::add(Resource resource, T* obj) -> void {
		auto idx         = static_cast<size_t>(resource.id());
		m_Resources[idx] = obj;
		m_ResourceStates[idx].store(EResourceState::loaded, std::memory_order_release);
		m_ResourceStates[idx].notify_all();

		for (auto* plugin : m_Plugins) {
			plugin->on_resource_loaded(resource.id(), resource.type());
		}
	}

	template <typename T, EResource ResourceType>
	template <typename... Args>
	requires(std::is_constructible_v<T, Args...>)
	auto ResourceContainer<T, ResourceType>::emplace(Resource resource, Args&&... args) -> void {
		auto idx         = static_cast<size_t>(resource.id());
		m_Resources[idx] = new T(std::forward<Args>(args)...);
		m_ResourceStates[idx].store(EResourceState::loaded, std::memory_order_release);
		m_ResourceStates[idx].notify_all();

		for (auto* plugin : m_Plugins) {
			plugin->on_resource_loaded(resource.id(), resource.type());
		}
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::remove(Resource resource) -> void {
		auto idx = static_cast<size_t>(resource.id());

		m_ResourceStates[idx].store(EResourceState::invalid, std::memory_order_release);

		delete m_Resources[idx];
		m_Resources[idx] = nullptr;

		std::lock_guard lock(m_IndexMutex);
		m_FreeIDs.push(resource.id());

		for (auto* plugin : m_Plugins) {
			plugin->on_resource_unloaded(resource.id(), resource.type());
		}
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::fail(Resource resource) -> void {
		auto idx = static_cast<size_t>(resource.id());
		m_ResourceStates[idx].store(EResourceState::failed, std::memory_order_release);
		m_ResourceStates[idx].notify_all();
		for (auto* plugin : m_Plugins) {
			if (!plugin->on_resource_failed(resource.id(), resource.type())) {
				std::println("failed to load resource: [id: {}, type: {}]", resource.id(), resource.type());
				ABY_RHI_DEBUG_BREAK();
			}
		}
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::wait_for(Resource resource) -> bool {
		auto idx   = static_cast<size_t>(resource.id());
		auto state = m_ResourceStates[idx].load(std::memory_order_acquire);

		while (state == EResourceState::loading) {
			// m_ResourceStates[idx].wait(state);
			state = m_ResourceStates[idx].load(std::memory_order_acquire);
		}

		return state == EResourceState::loaded;
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::is_loaded(Resource resource) -> bool {
		auto idx = static_cast<size_t>(resource.id());
		return m_ResourceStates[idx].load(std::memory_order_acquire) == EResourceState::loaded;
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::state(Resource resource) -> EResourceState {
		return m_ResourceStates[static_cast<size_t>(resource.id())].load(std::memory_order_acquire);
	}

	template <typename T, EResource ResourceType>
	auto ResourceContainer<T, ResourceType>::operator[](Resource resource) -> T* {
		if (resource.type() != ResourceType) {
			return nullptr;
		}
		auto idx = static_cast<size_t>(resource.id());

		auto state = m_ResourceStates[idx].load(std::memory_order_acquire);

		if (state != EResourceState::loaded) {
			return nullptr;
		}
		return m_Resources[idx];
	}

} // namespace aby::rhi

namespace aby::rhi {

	template <typename T, EResource ResourceType>
	ResourcePtr<T, ResourceType>::ResourcePtr() = default;

	template <typename T, EResource ResourceType>
	ResourcePtr<T, ResourceType>::ResourcePtr(std::nullptr_t) :
	    Resource(ResourceType, ResourceID::invalid),
	    m_Cached(nullptr),
	    m_Container(nullptr) {
	}

	template <typename T, EResource ResourceType>
	ResourcePtr<T, ResourceType>::ResourcePtr(ResourceID id, ResourceContainer<T, ResourceType>* container) :
	    Resource(ResourceType, id),
	    m_Container(container) {
	}

	template <typename T, EResource ResourceType>
	auto ResourcePtr<T, ResourceType>::operator->() -> T* {
		if (!m_Cached) {
			if (!m_Container->wait_for(*this))
				return nullptr;

			m_Cached = (*m_Container)[*this];
		}

		return m_Cached;
	}

	template <typename T, EResource ResourceType>
	auto ResourcePtr<T, ResourceType>::operator*() -> T& {
		auto* ptr = get();
		if (ptr == nullptr) {
			// cant assert here because context.hpp needs to know about resource
			std::fprintf(stderr, "attempted to dereference a null ResourcePtr\n");
			ABY_RHI_DEBUG_BREAK();
		}
		return *ptr;
	}

	template <typename T, EResource ResourceType>
	auto ResourcePtr<T, ResourceType>::get() -> T* {
		return operator->();
	}

	template <typename T, EResource ResourceType>
	ResourcePtr<T, ResourceType>::operator bool() const {
		return this->id() != ResourceID::invalid;
	}

} // namespace aby::rhi
