#include "interfaces/default_job_system.hpp"

#include "context.hpp"

namespace aby::rhi {

	DefaultJobSystem::DefaultJobSystem() {
		auto hw = std::thread::hardware_concurrency();

		if (hw == 0)
			hw = 1;

		m_ThreadCount = std::max(1u, (hw * 2) / 3);

		m_Arena.initialize(static_cast<int>(m_ThreadCount));
	}

	DefaultJobSystem::~DefaultJobSystem() noexcept {
		destroy();
	}

	auto DefaultJobSystem::name() -> std::string_view {
		return "DefaultJobSystem";
	}

	auto DefaultJobSystem::thread_count() -> size_t {
		return m_ThreadCount;
	}

	auto DefaultJobSystem::add_job(EJobPriority priority, Job&& job) -> void {
		if (!bRunning.load(std::memory_order_relaxed))
			return;

		m_Arena.execute([this, priority, job = std::move(job)]() {
			m_Tasks.run([job = std::move(job)]() {
				job();
			});
		});
	}

	auto DefaultJobSystem::destroy() -> void {
		if (!bRunning.exchange(false))
			return;

		m_Tasks.wait();
		m_Arena.terminate();
	}

} // namespace aby::rhi
