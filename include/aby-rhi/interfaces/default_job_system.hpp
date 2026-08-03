#pragma once

#include "interfaces/interfaces.hpp"

#include <array>
#include <condition_variable>
#include <mutex>
#include <thread>

#ifdef _MSC_VER
#	include <concurrent_queue.h>
namespace aby::rhi {

	template <typename T>
	using ConcurrentQueue = concurrency::concurrent_queue<T>;

}
#else
#	error "implement concurrent_queue for other compilers"
#endif

namespace aby::rhi {

	struct DefaultJobQueue {
		std::thread thread;
		ConcurrentQueue<IJobSystem::Job> low;
		ConcurrentQueue<IJobSystem::Job> medium;
		ConcurrentQueue<IJobSystem::Job> high;
		ConcurrentQueue<IJobSystem::Job> critical;
		std::atomic_bool running{ true };
		std::mutex mutex;
		std::condition_variable cv;
	};

	class DefaultJobSystem : public IJobSystem {
	private:
		static constexpr size_t MAX_QUEUES = static_cast<size_t>(EJobQueue::max_queues);
	public:
		DefaultJobSystem();

		auto name() -> std::string_view override;

		auto add_job(EJobQueue queue, EJobPriority priority, Job&& job) -> void override;
		auto destroy() -> void override;
	private:
		std::array<DefaultJobQueue, MAX_QUEUES> m_Queues;
	};

} // namespace aby::rhi
