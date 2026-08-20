#include "plugins/plugin.hpp"

namespace aby::rhi {

	auto Plugin::on_init(EInitTime init_time) -> bool {
		return true;
	}

	auto Plugin::on_begin() -> void {
	}

	auto Plugin::on_end() -> void {
	}

	auto Plugin::on_deinit(EDeinitTime deinit_time) -> void {
	}

	auto Plugin::on_resource_loaded(ResourceID resource, EResource type) -> void {
	}

	auto Plugin::on_resource_unloaded(ResourceID resource, EResource type) -> void {
	}

	auto Plugin::on_resource_failed(ResourceID resource, EResource type) -> bool {
		return true;
	}

} // namespace aby::rhi
