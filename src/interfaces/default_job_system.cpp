#include "interfaces/default_job_system.hpp"

#include "context.hpp"

namespace aby::rhi {

	DefaultJobSystem::DefaultJobSystem() :
	    m_NextQueue(0) {
		auto hw = std::thread::hardware_concurrency();
		if (hw == 0)
			hw = 1;
		auto workers = std::max(1u, (hw * 2) / 3);
		m_Queues.reserve(workers);

		for (size_t i = 0; i < workers; i++) {
			m_Queues.emplace_back(std::make_unique<DefaultJobQueue>());

			auto* queue = m_Queues.back().get();

			queue->thread = std::thread([queue, i]() {
				Job job;

				while (true) {
					bool finished_any_work = false;

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

					if (finished_any_work)
						continue;

					std::unique_lock lock(queue->mutex);

					queue->cv.wait(lock, [&] {
						return !queue->running.load() ||
						       !queue->critical.empty() ||
						       !queue->high.empty() ||
						       !queue->medium.empty() ||
						       !queue->low.empty();
					});

					if (!queue->running.load())
						break;
				}
			});
		}
	}

	DefaultJobSystem::~DefaultJobSystem() {
		destroy();
	}

	auto DefaultJobSystem::name() -> std::string_view {
		return "DefaultJobSystem";
	}

	auto DefaultJobSystem::thread_count() -> size_t {
		return m_Queues.size();
	}

	auto DefaultJobSystem::add_job(EJobPriority priority, Job&& job) -> void {
		auto& q = m_Queues[m_NextQueue];

		switch (priority) {
			case EJobPriority::critical:
				q->critical.push(std::move(job));
				break;
			case EJobPriority::high:
				q->high.push(std::move(job));
				break;
			case EJobPriority::medium:
				q->medium.push(std::move(job));
				break;
			case EJobPriority::low:
				q->low.push(std::move(job));
				break;
		}

		q->cv.notify_one();

		m_NextQueue = (m_NextQueue + 1) % m_Queues.size();
	}

	auto DefaultJobSystem::destroy() -> void {
		for (size_t i = 0; i < m_Queues.size(); i++) {
			auto& queue = m_Queues[i];
			queue->running.store(false);
			queue->cv.notify_one();
			if (queue->thread.joinable()) {
				queue->thread.join();
			}
		}
	}

} // namespace aby::rhi
