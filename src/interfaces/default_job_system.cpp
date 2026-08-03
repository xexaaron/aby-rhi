#include "interfaces/default_job_system.hpp"

namespace aby::rhi {

	DefaultJobSystem::DefaultJobSystem() {
		for (size_t i = 0; i < m_Queues.size(); i++) {
			auto* queue   = &m_Queues[i];
			queue->thread = std::thread([queue = queue]() {
				bool finished_any_work = false;
				Job job;

				while (true) {
					while (queue->critical.try_pop(job)) {
						job();
						finished_any_work = true;
					}
					while (queue->high.try_pop(job)) {
						job();
						finished_any_work = true;
					}
					while (queue->medium.try_pop(job)) {
						job();
						finished_any_work = true;
					}
					while (queue->low.try_pop(job)) {
						job();
						finished_any_work = true;
					}

					if (finished_any_work) {
						continue;
					}

					std::unique_lock lock(queue->mutex);
					queue->cv.wait(lock, [&] {
						return !queue->running.load() ||
						       !queue->critical.empty() ||
						       !queue->high.empty() ||
						       !queue->medium.empty() ||
						       !queue->low.empty();
					});

					if (!queue->running.load()) {
						break;
					}
				}
			});
		}
	}

	auto DefaultJobSystem::name() -> std::string_view {
		return "DefaultJobSystem";
	}

	auto DefaultJobSystem::add_job(EJobQueue queue, EJobPriority priority, Job&& job) -> void {
		auto queue_idx = static_cast<size_t>(queue);
		auto& q        = m_Queues[queue_idx];

		switch (priority) {
			case EJobPriority::low: {
				q.low.push(std::move(job));
				break;
			}
			case EJobPriority::medium: {
				q.medium.push(std::move(job));
				break;
			}
			case EJobPriority::high: {
				q.high.push(std::move(job));
				break;
			}
			case EJobPriority::critical: {
				q.critical.push(std::move(job));
				break;
			}
		}
	}

	auto DefaultJobSystem::destroy() -> void {
		for (size_t i = 0; i < m_Queues.size(); i++) {
			auto& queue = m_Queues[i];
			queue.running.store(false);
			queue.cv.notify_one();
			if (queue.thread.joinable()) {
				queue.thread.join();
			}
		}
	}

} // namespace aby::rhi
