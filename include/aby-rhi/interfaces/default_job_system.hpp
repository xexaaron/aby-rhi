#pragma once

#include "interfaces/interfaces.hpp"

#include <condition_variable>
#include <mutex>
#include <oneapi/tbb/task_arena.h>
#include <oneapi/tbb/task_group.h>
#include <tbb/concurrent_queue.h>
#include <tbb/task_arena.h>
#include <tbb/task_group.h>
#include <thread>

namespace aby::rhi {

	class DefaultJobSystem : public IJobSystem {
	public:
		DefaultJobSystem();
		~DefaultJobSystem() noexcept;

		auto name() -> std::string_view override;

		auto thread_count() -> size_t override;
		auto add_job(EJobPriority priority, Job&& job) -> void override;
		auto destroy() -> void override;
	private:
		tbb::task_arena m_Arena;
		tbb::task_group m_Tasks;
		size_t m_ThreadCount;
		std::atomic<bool> bRunning{ true };
	};

} // namespace aby::rhi
