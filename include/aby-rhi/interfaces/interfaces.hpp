#pragma once
#include "common.hpp"

#include <filesystem>
#include <functional>
#include <span>
#include <string>
#include <string_view>

namespace aby::rhi {

	namespace fs = std::filesystem;

	/**
     * @brief Base class for interfaces providing a common subset of debugging information
    */
	class IInterface {
	public:
		virtual ~IInterface()                   = default;
		/**
		 * @brief Debug class name
		 */
		virtual auto name() -> std::string_view = 0;
	private:
	};

	/**
     * @brief ILogger message for passing pre-formatted messages to the application
    */
	class ILogger : public IInterface {
	public:
		auto name() -> std::string_view override;
		/**
		 * @brief log a message 
		 * @param level the level of the log message
		 * @param msg the message data
		 */
		virtual auto log(ELogLevel level, const std::string& msg) -> void = 0;
	private:
	};

	/**
     * @brief IAllocator interface for aligned memory management
	*/
	class IAllocator : public IInterface {
	public:
		auto name() -> std::string_view override;
		/**
		 * @brief Allocate a chunk of memory
		 * @param bytes the requested size
		 * @param alignment a power of 2. alignment must be respected at all times.
		 * @param type The type of allocation. (may be ignored)
		 */
		virtual auto alloc(size_t bytes, size_t alignment, EAllocation type) -> void*              = 0;
		/**
		 * @brief Reallocate a chunk of memory.
		 * @param ptr the old pointer
		 * @param bytes the requested size
		 * @param alignment a power of 2. alignment must be respected at all times.
		 * @param type The type of allocation. (may be ignored)
		 */
		virtual auto realloc(void* ptr, size_t bytes, size_t alignment, EAllocation type) -> void* = 0;
		/**
		 * @brief Free a chunk of memory
		 * @param ptr the pointer to free
		 * @param type (may be ignored)
		 */
		virtual auto free(void* ptr, EAllocation type) -> void                                     = 0;
	private:
	};

	/**
     * @brief IFileIO interface for reading/writing data.
	 * 		  Controls the working directory and cache directory.
     */
	class IFileIO : public IInterface {
	public:
		auto name() -> std::string_view override;
		/**
		 * @brief Concatenates a path with the cache directory
		 * @param rel_path path relative to the cache directory
		 * @return absolute path
		 */
		auto cache_path(const fs::path& rel_path) -> fs::path;
		/**
		 * @brief Concatenates a path with the cache directory
		 * @param rel_path path relative to the cache directory
		 * @param append_ext an extension to append to the relative path
		 * @return absolute path
		 */
		auto cache_path(const fs::path& rel_path, const std::string& append_ext) -> fs::path;
		/**
		 * @brief Check if a cache path exists
		 * @param rel_path path relative to the cache directory.
		 * @return true if exists, otherwise false
		 */
		auto cache_path_exists(const fs::path& rel_path) -> bool;
		/**
		 * @brief Check if a cache path exists
		 * @param rel_path path relative to the cache directory.
		 * @param append_ext an extension to append to the relative path.
		 * @return true if exists, otherwise false
		 */
		auto cache_path_exists(const fs::path& rel_path, const std::string& append_ext) -> bool;
		/**
		 * @brief Concatenates a path with the CWD
		 * @param rel_path path relative to the CWD
		 * @return absolute path
		 */
		auto path(const fs::path& rel_path) -> fs::path;
		/**
		 * @brief Checks if a relative path exists inside the CWD
		 * @param rel_path path relative to the CWD
		 * @return true if exists, otherwise false
		 */
		auto exists(const fs::path& rel_path) -> bool;
		/**
		 * @brief Set the cache directory. Used mainly for when using the DefaultFileIO.
		 */
		virtual auto set_cwd(const fs::path& path) -> void                               = 0;
		/**
		 * @brief set the cache directory. Used mainly for when using the DefaultFileIO.
		 */
		virtual auto set_cache_dir(const fs::path& path) -> void                         = 0;
		/**
		 * @brief The current working directory from which files can be located via relative paths.
		 */
		virtual auto cwd() const -> const fs::path&                                      = 0;
		/**
		 * @brief The directory which cached files can be located from relative paths.
		 */
		virtual auto cache_dir() const -> const fs::path&                                = 0;
		/**
		 * @brief Read an entire file.
		 * @param rel_path path relative to the cwd
		 * @param data uint8_t byte vector. (this function will reserve space in the vector)
		 * @return true if success, otherwise false.
		 */
		virtual auto read(const fs::path& rel_path, std::vector<uint8_t>* data) -> bool  = 0;
		/**
		 * @brief Read an entire file
		 * @param rel_path path relative to the cwd
		 * @param data uint32_t vector (this function will reserve space in the vector)
		 * @return true if success, otherwise false
		 */
		virtual auto read(const fs::path& rel_path, std::vector<uint32_t>* data) -> bool = 0;
		/**
		 * @brief Write to a file. (expects to truncate)
		 * @param rel_path path relative to the cwd
		 * @param data uint8_t span
		 * @return true if success, otherwise false
		 */
		virtual auto write(const fs::path& rel_path, std::span<uint8_t> data) -> bool    = 0;
		/**
		 * @brief Write to a file. (expects to truncate)
		 * @param rel_path path relative to the cwd
		 * @param data uint32_t span
		 * @return true if success, otherwise false
		 */
		virtual auto write(const fs::path& rel_path, std::span<uint32_t> data) -> bool   = 0;
	private:
	};

	/**
	 * @brief Job system for schedulig work onto different threads
	 * 		  with a priority system. 
	 * @note The default job system does not use weighted priority sorting
	 *  	 It will go in the order of highest priority to lowest priority
	 * 	     which may end up blocking lower priority tasks.  
	 */
	class IJobSystem : public IInterface {
	public:
		using Job = std::function<void()>;

		auto name() -> std::string_view override;

		/**
		 * @brief Get the number of threads that jobs can be run on.
		 * @note The default job system requests 2/3 of the available threads. 
		 * 		 This is probably overkill.
		 */
		virtual auto thread_count() -> size_t = 0;

		/**
		 * @brief Add a job to be ran asynchronously 
		 * @param priority The order in which the jobs should be completed.
		 */
		virtual auto add_job(EJobPriority priority, Job&& job) -> void = 0;

		/**
		 * @brief Should cleanup any resources the JobSystem uses
		 * 	      and finish all jobs currently running. The jobs must finish
		 * 		  because they could be jobs such as caching data.
		 */
		virtual auto destroy() -> void = 0;
	private:
	};

} // namespace aby::rhi
