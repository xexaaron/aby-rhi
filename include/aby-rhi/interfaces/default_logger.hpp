#pragma once
#include "interfaces/interfaces.hpp"

namespace aby::rhi {

    class DefaultLogger final : public ILogger {
    public:
        auto name() -> std::string_view override;
        auto log(ELogLevel level, const std::string& msg) -> void override;
    private:
    };

}