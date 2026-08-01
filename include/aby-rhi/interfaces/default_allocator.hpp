#pragma once
#include "interfaces/interfaces.hpp"

namespace aby::rhi {

	class DefaultAllocator final : public IAllocator {
	public:
		auto name() -> std::string_view override;
		auto alloc(size_t bytes, size_t alignment, EAllocation type) -> void* override;
		auto realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* override;
		auto free(void* ptr, EAllocation type) -> void override;
	private:
	};

} // namespace aby::rhi
