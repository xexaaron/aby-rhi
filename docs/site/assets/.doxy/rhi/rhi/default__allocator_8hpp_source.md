

# File default\_allocator.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**interfaces**](dir_6277bbb7ca6e5761e568397cf4fde752.md) **>** [**default\_allocator.hpp**](default__allocator_8hpp.md)

[Go to the documentation of this file](default__allocator_8hpp.md)


```C++
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
```


