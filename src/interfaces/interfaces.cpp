#include "interfaces/interfaces.hpp"

namespace aby::rhi {

	auto ILogger::name() -> std::string_view {
		return "ILogger";
	};

	auto IAllocator::name() -> std::string_view {
		return "IAllocator";
	};

	auto IFileIO::name() -> std::string_view {
		return "IFileIO";
	};

	auto IFileIO::cache_path(const fs::path& rel_path) -> fs::path {
		return cache_dir() / rel_path;
	}

	auto IFileIO::cache_path(const fs::path& rel_path, const std::string& append_ext) -> fs::path {
		return cache_path(rel_path.string().append(append_ext));
	}

	auto IFileIO::cache_path_exists(const fs::path& rel_path) -> bool {
		return fs::exists(cache_path(rel_path));
	}

	auto IFileIO::cache_path_exists(const fs::path& rel_path, const std::string& append_ext) -> bool {
		return fs::exists(cache_path(rel_path, append_ext));
	}


	auto IFileIO::path(const fs::path& rel_path) -> fs::path {
		return cwd() / rel_path;
	}

	auto IFileIO::exists(const fs::path& rel_path) -> bool {
		return fs::exists(path(rel_path));
	}

	auto IJobSystem::name() -> std::string_view {
		return "IJobSystem";
	};

} // namespace aby::rhi
