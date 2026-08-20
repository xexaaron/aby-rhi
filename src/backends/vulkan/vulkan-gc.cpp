#include "backends/vulkan/vulkan-gc.hpp"

#include "context.hpp"

namespace aby::rhi::vulkan {

	auto GarbageCollector::push(Dtor&& dtor) -> void {
		aby_rhi_dbg("adding dtor: {}", (void*)&dtor);
		m_Dtors.emplace(std::move(dtor));
	}

	auto GarbageCollector::run() -> void {
		aby_rhi_dbg("executing dtors");
		while (!m_Dtors.empty()) {
			auto& dtor = m_Dtors.top();
			if (dtor) {
				aby_rhi_dbg("executing dtor: {}", (void*)&dtor);
				dtor();
			}
			m_Dtors.pop();
		}
	}

} // namespace aby::rhi::vulkan
