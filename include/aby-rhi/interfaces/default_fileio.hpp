#pragma once
#include "interfaces/interfaces.hpp"

namespace aby::rhi {

    class DefaultFileIO final : public IFileIO {
    public:
        auto read(const fs::path& path, std::vector<uint8_t>* data) -> bool override;
        auto read(const fs::path& path, std::vector<uint32_t>* data) -> bool override;
        auto write(const fs::path& path, std::span<uint8_t> data) -> bool override;
    private:
    };

}