

# File interfaces.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**interfaces**](dir_6277bbb7ca6e5761e568397cf4fde752.md) **>** [**interfaces.hpp**](interfaces_8hpp.md)

[Go to the documentation of this file](interfaces_8hpp.md)


```C++


#pragma once
#include "common.hpp"

#include <filesystem>
#include <functional>
#include <span>
#include <string>
#include <string_view>

namespace aby::rhi {

    namespace fs = std::filesystem;

    class IInterface {
    public:
        virtual ~IInterface()                   = default;
        virtual auto name() -> std::string_view = 0;
    private:
    };

    class ILogger : public IInterface {
    public:
        auto name() -> std::string_view override;
        virtual auto log(ELogLevel level, const std::string& msg) -> void = 0;
    private:
    };

    class IAllocator : public IInterface {
    public:
        auto name() -> std::string_view override;
        virtual auto alloc(size_t bytes, size_t alignment, EAllocation type) -> void*              = 0;
        virtual auto realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* = 0;
        virtual auto free(void* ptr, EAllocation type) -> void                                     = 0;
    private:
    };

    class IFileIO : public IInterface {
    public:
        auto name() -> std::string_view override;

        virtual auto set_cwd(const fs::path& path) -> void       = 0;
        virtual auto set_cache_dir(const fs::path& path) -> void = 0;
        virtual auto cwd() const -> const fs::path&              = 0;
        virtual auto cache_dir() const -> const fs::path&        = 0;

        virtual auto read(const fs::path& rel_path, std::vector<uint8_t>* data) -> bool  = 0;
        virtual auto read(const fs::path& rel_path, std::vector<uint32_t>* data) -> bool = 0;
        virtual auto write(const fs::path& rel_path, std::span<uint8_t> data) -> bool    = 0;
        virtual auto write(const fs::path& rel_path, std::span<uint32_t> data) -> bool   = 0;
    private:
    };

    class IJobSystem : public IInterface {
    public:
        using Job = std::function<void()>;
        auto name() -> std::string_view override;

        virtual auto thread_count() -> size_t = 0;

        virtual auto add_job(EJobPriority priority, Job&& job) -> void = 0;

        virtual auto destroy() -> void = 0;
    private:
    };

} // namespace aby::rhi
```


