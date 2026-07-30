#pragma once
#include "common.hpp"
#include <functional>
#include <stack>

namespace aby::rhi::vulkan {

    class GarbageCollector {
    public:
        using Dtor = std::function<void()>;
        
        auto push(Dtor&& dtor) -> void;
        auto run() -> void;
    private:
        std::stack<Dtor> m_Dtors;
    };

};