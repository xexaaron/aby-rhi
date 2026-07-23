#pragma once
#include "common.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

namespace aby::rhi {

    enum class ELogLevel {
        debug,
        trace,
        info, 
        warn,
        error,
        fatal,
    };

    class IInterface {
    public:
        virtual auto name() -> std::string_view = 0;
    private:
    };

    class ILogger : public IInterface {
    public:
        auto name() -> std::string_view override { return "ILogger"; };
        virtual auto log(ELogLevel level, const std::string& msg) -> void = 0;
    private:
    };

    class IAllocator : public IInterface {
    public:
        auto name() -> std::string_view override { return "IAllocator"; };
        virtual auto alloc(size_t bytes, size_t alignment, EAllocation type) -> void* = 0;
        virtual auto realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* = 0;
        virtual auto free(void* ptr) -> void = 0;
        /// @brief optional usage tracking. 
        virtual auto usage() -> std::unordered_map<EAllocation, size_t> { return {}; } 
    private:
    };

}