#include "interfaces/default_allocator.hpp"

namespace aby::rhi {

	auto DefaultAllocator::name() -> std::string_view {
		return "DefaultAllocator";
	};

	auto DefaultAllocator::alloc(size_t bytes, size_t alignment, EAllocation type) -> void* {
#ifdef _MSC_VER
#	ifndef _NDEBUG
		return _aligned_malloc_dbg(bytes, alignment, __FILE__, __LINE__);
#	else
		return _aligned_malloc(bytes, alignment);
#	endif
#else
		return std::aligned_alloc(alignment, bytes);
#endif
	};

	auto DefaultAllocator::realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* {
#ifdef _MSC_VER
#	ifndef _NDEBUG
		return _aligned_realloc_dbg(ptr, bytes, alignment, __FILE__, __LINE__);
#	else
		return _aligned_realloc(ptr, bytes, alignment);
#	endif
#else
		return std::aligned_realloc(ptr, alignment, bytes);
#endif
	};

	auto DefaultAllocator::free(void* ptr, EAllocation type) -> void {
#ifdef _MSC_VER
#	ifndef _NDEBUG
		_aligned_free_dbg(ptr);
#	else
		_aligned_free(ptr);
#	endif
#else
		std::free(ptr);
#endif
	};

} // namespace aby::rhi
