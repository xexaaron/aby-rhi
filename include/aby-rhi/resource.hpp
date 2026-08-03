#pragma once
#include "common.hpp"

#include <deque>
#include <mutex>
#include <queue>
#include <vector>

namespace aby::rhi {

	enum class EResource : uint32_t {
		none,
		texture,
		shader,
	};

	enum class EResourceState : uint16_t {
		invalid,
		loading,
		loaded,
		failed,
	};

	enum class ResourceID : uint32_t {
		invalid = UINT32_MAX,
	};

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
		auto state() const -> EResourceState;
		auto id() const -> ResourceID;

		operator ResourceID() const;
		operator std::pair<EResource, ResourceID>() const;
	private:
		EResource m_Type;
		ResourceID m_ID;
	};

	/**
	 * @brief A resource container that owns the resources and performs cleanup
	 * @tparam T the type of resource.
	 * @tparam resource_type The type of resource that is owned. Used for runtime checking and resource creation
	 *                       to ensure that resources owned by another container arent accessed using this container.
	 * @warning The container does not do bounds checking itself.
	 */
	template <typename T, EResource resource_type>
	class ResourceContainer {
	public:
		~ResourceContainer();

		/**
		 * @brief Reserve a resource slot in the container. This resource can be passed around before the creation
		 *        of the actual data.
		 * @return A resource handle that can later be passed to add/emplace.
		 */
		auto reserve() -> Resource;

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
		std::mutex m_IndexMutex;
	};

} // namespace aby::rhi

namespace aby::rhi {

	template <typename T, EResource resource_type>
	ResourceContainer<T, resource_type>::~ResourceContainer() {
		for (auto* resource : m_Resources) {
			delete resource;
		}
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::reserve() -> Resource {
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

		return Resource(resource_type, id);
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::add(Resource resource, T* obj) -> void {
		auto idx = static_cast<size_t>(resource.id());

		m_Resources[idx] = obj;
		m_ResourceStates[idx].store(EResourceState::loaded, std::memory_order_release);
		m_ResourceStates[idx].notify_all();
	}

	template <typename T, EResource resource_type>
	template <typename... Args>
	requires(std::is_constructible_v<T, Args...>)
	auto ResourceContainer<T, resource_type>::emplace(Resource resource, Args&&... args) -> void {
		auto idx = static_cast<size_t>(resource.id());

		m_Resources[idx] = new T(std::forward<Args>(args)...);
		m_ResourceStates[idx].store(EResourceState::loaded, std::memory_order_release);
		m_ResourceStates[idx].notify_all();
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::remove(Resource resource) -> void {
		auto idx = static_cast<size_t>(resource.id());

		m_ResourceStates[idx].store(EResourceState::invalid, std::memory_order_release);

		delete m_Resources[idx];
		m_Resources[idx] = nullptr;

		std::lock_guard lock(m_IndexMutex);
		m_FreeIDs.push(resource.id());
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::fail(Resource resource) -> void {
		auto idx = static_cast<size_t>(resource.id());
		m_ResourceStates[idx].store(EResourceState::failed, std::memory_order_release);
		m_ResourceStates[idx].notify_all();
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::wait_for(Resource resource) -> bool {
		auto idx = static_cast<size_t>(resource.id());

		auto state = m_ResourceStates[idx].load(
		    std::memory_order_acquire);

		while (state == EResourceState::loading) {
			m_ResourceStates[idx].wait(state);
			state = m_ResourceStates[idx].load(std::memory_order_acquire);
		}

		return state == EResourceState::loaded;
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::is_loaded(Resource resource) -> bool {
		auto idx = static_cast<size_t>(resource.id());
		return m_ResourceStates[idx].load(std::memory_order_acquire) == EResourceState::loaded;
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::state(Resource resource) -> EResourceState {
		return m_ResourceStates[static_cast<size_t>(resource.id())].load(std::memory_order_acquire);
	}

	template <typename T, EResource resource_type>
	auto ResourceContainer<T, resource_type>::operator[](Resource resource) -> T* {
		if (resource.type() != resource_type) {
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
