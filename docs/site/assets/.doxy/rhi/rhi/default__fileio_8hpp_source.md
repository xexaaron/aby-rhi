

# File default\_fileio.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**interfaces**](dir_6277bbb7ca6e5761e568397cf4fde752.md) **>** [**default\_fileio.hpp**](default__fileio_8hpp.md)

[Go to the documentation of this file](default__fileio_8hpp.md)


```C++
#pragma once
#include "interfaces/interfaces.hpp"

namespace aby::rhi {

    class DefaultFileIO final : public IFileIO {
    public:
        auto set_cwd(const fs::path& path) -> void override;
        auto set_cache_dir(const fs::path& path) -> void override;
        auto cwd() const -> const fs::path& override;
        auto cache_dir() const -> const fs::path& override;

        auto read(const fs::path& rel_path, std::vector<uint8_t>* data) -> bool override;
        auto read(const fs::path& rel_path, std::vector<uint32_t>* data) -> bool override;
        auto write(const fs::path& rel_path, std::span<uint8_t> data) -> bool override;
        auto write(const fs::path& rel_path, std::span<uint32_t> data) -> bool override;
    private:
        fs::path m_CWD;
        fs::path m_CacheDir;
    };

} // namespace aby::rhi
```


