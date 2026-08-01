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

} // namespace aby::rhi
