#include "backends/vulkan/vulkan-gc.hpp"

#include "context.hpp"

namespace aby::rhi::vulkan {

	auto GarbageCollector::push(Dtor&& dtor) -> void {
		m_Dtors.emplace(std::move(dtor));
	}

	auto GarbageCollector::run() -> void {
		while (!m_Dtors.empty()) {
			auto& dtor = m_Dtors.top();
			if (dtor) {
				dtor();
			}
			m_Dtors.pop();
		}
	}

} // namespace aby::rhi::vulkan
