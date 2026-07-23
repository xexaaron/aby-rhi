#pragma once
#include <string>

namespace aby::rhi {

    enum class ELogLevel {
        trace,
        info, 
        warn,
        error,
        fatal,
    };

    class IInterface {
    public:
    private:
    };

    class ILogger : public IInterface {
    public:
        virtual auto log(ELogLevel level, const std::string& msg) -> void = 0;
    private:
    };

}