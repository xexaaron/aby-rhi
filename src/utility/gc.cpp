#include "utility/gc.hpp"

namespace aby::rhi {

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

} // namespace aby::rhi
