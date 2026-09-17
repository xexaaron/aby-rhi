#pragma once
#include "common.hpp"

#include <functional>
#include <stack>

namespace aby::rhi {

	class ABY_RHI_API GarbageCollector {
	public:
		using Dtor = std::function<void()>;

		auto push(Dtor&& dtor) -> void;
		auto run() -> void;
	private:
		std::stack<Dtor> m_Dtors;
	};

} // namespace aby::rhi
