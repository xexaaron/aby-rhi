/**
 * @brief A set of interfaces that can be set by the user to modify parts of the library.
 *
 * @param IInterface Do not use directly. it is the base class of interfaces.
 * @param ILogger    Logging interface to pass pre-formatted messages to the application
 * @param IAllocator Allocator interface used in the backed and for other allocations. 
 *   
*/

#pragma once
#include "common.hpp"
#include <string>
#include <string_view>
#include <filesystem>
#include <span>

namespace aby::rhi {

    namespace fs = std::filesystem;

    /**
     * @brief Base class for interfaces providing a common subset of debugging information
    */
    class IInterface {
    public:
        virtual ~IInterface() = default;
        virtual auto name() -> std::string_view = 0;
    private:
    };

    /**
     * @brief ILogger message for passing pre-formatted messages to the application
    */
    class ILogger : public IInterface {
    public:
        auto name() -> std::string_view override;
        virtual auto log(ELogLevel level, const std::string& msg) -> void = 0;
    private:
    };

    /**
     * @brief IAllocator interface for alloc/free/realloc
     * @param type can be ignored, it its purely for debuggability/memory usage tracking 
     * @param alignment Aligment must be respected at all times. The DefaultAllocator does this already.
    */
    class IAllocator : public IInterface {
    public:
        auto name() -> std::string_view override;
        virtual auto alloc(size_t bytes, size_t alignment, EAllocation type) -> void* = 0;
        virtual auto realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* = 0;
        virtual auto free(void* ptr, EAllocation type) -> void = 0;
    private:
    };

    /**
     * @brief IFileIO interface for reading/writing data. Used for reading/writing to/from shaders/textures
     * @param rel_path A relative path starting from the currently set working directory
     */
    class IFileIO : public IInterface {
    public:
        auto name() -> std::string_view override;
        
        virtual auto set_cwd(const fs::path& path) -> void = 0;
        virtual auto set_cache_dir(const fs::path& path) -> void = 0;
        virtual auto cwd() const -> const fs::path& = 0;
        virtual auto cache_dir() const -> const fs::path& = 0;
        
        virtual auto read(const fs::path& rel_path, std::vector<uint8_t>* data) -> bool = 0;
        virtual auto read(const fs::path& rel_path, std::vector<uint32_t>* data) -> bool = 0;
        virtual auto write(const fs::path& rel_path, std::span<uint8_t> data) -> bool = 0;
        virtual auto write(const fs::path& rel_path, std::span<uint32_t> data) -> bool = 0;
    private:
    };

}