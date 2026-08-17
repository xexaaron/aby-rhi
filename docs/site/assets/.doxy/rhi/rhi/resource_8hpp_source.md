

# File resource.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**resource.hpp**](resource_8hpp.md)

[Go to the documentation of this file](resource_8hpp.md)


```C++
#pragma once
#include "common.hpp"

#include <deque>
#include <mutex>
#include <queue>
#include <vector>

namespace aby::rhi {

    class Resource {
    public:
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

    template <typename T, EResource ResourceType>
    class ResourceContainer {
    public:
        ~ResourceContainer();

        auto reserve() -> Resource;

        auto add(Resource resource, T* obj) -> void;

        template <typename... Args>
        requires(std::is_constructible_v<T, Args...>)
        auto emplace(Resource resource, Args&&... args) -> void;

        auto remove(Resource resource) -> void;

        auto fail(Resource resource) -> void;

        auto wait_for(Resource resource) -> bool;

        auto is_loaded(Resource resource) -> bool;

        auto state(Resource resource) -> EResourceState;

        auto operator[](Resource resource) -> T*;
    private:
        std::vector<T*> m_Resources;
        std::queue<ResourceID> m_FreeIDs;
        std::deque<std::atomic<EResourceState>> m_ResourceStates;
        std::mutex m_IndexMutex;
    };

    template <typename T, EResource ResourceType>
    class ResourcePtr : public Resource {
    public:
        ResourcePtr();
        ResourcePtr(std::nullptr_t);
        ResourcePtr(ResourceID id, ResourceContainer<T, ResourceType>* container);

        auto operator->() -> T*;
        auto operator*() -> T&;
        auto get() -> T*;

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
        for (auto* resource : m_Resources) {
            delete resource;
        }
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
    }

    template <typename T, EResource ResourceType>
    template <typename... Args>
    requires(std::is_constructible_v<T, Args...>)
    auto ResourceContainer<T, ResourceType>::emplace(Resource resource, Args&&... args) -> void {
        auto idx         = static_cast<size_t>(resource.id());
        m_Resources[idx] = new T(std::forward<Args>(args)...);
        m_ResourceStates[idx].store(EResourceState::loaded, std::memory_order_release);
        m_ResourceStates[idx].notify_all();
    }

    template <typename T, EResource ResourceType>
    auto ResourceContainer<T, ResourceType>::remove(Resource resource) -> void {
        auto idx = static_cast<size_t>(resource.id());

        m_ResourceStates[idx].store(EResourceState::invalid, std::memory_order_release);

        delete m_Resources[idx];
        m_Resources[idx] = nullptr;

        std::lock_guard lock(m_IndexMutex);
        m_FreeIDs.push(resource.id());
    }

    template <typename T, EResource ResourceType>
    auto ResourceContainer<T, ResourceType>::fail(Resource resource) -> void {
        auto idx = static_cast<size_t>(resource.id());
        m_ResourceStates[idx].store(EResourceState::failed, std::memory_order_release);
        m_ResourceStates[idx].notify_all();
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
```


