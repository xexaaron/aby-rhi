

# File default\_job\_system.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**interfaces**](dir_6277bbb7ca6e5761e568397cf4fde752.md) **>** [**default\_job\_system.hpp**](default__job__system_8hpp.md)

[Go to the documentation of this file](default__job__system_8hpp.md)


```C++
#pragma once

#include "interfaces/interfaces.hpp"

#include <array>
#include <condition_variable>
#include <mutex>
#include <thread>

#ifdef _MSC_VER
#   include <concurrent_queue.h>
namespace aby::rhi {

    template <typename T>
    using ConcurrentQueue = concurrency::concurrent_queue<T>;

}
#else
#   error "implement concurrent_queue for other compilers"
#endif

namespace aby::rhi {

    struct DefaultJobQueue {
        std::atomic_bool running{ true };
        std::thread thread;
        ConcurrentQueue<IJobSystem::Job> low;
        ConcurrentQueue<IJobSystem::Job> medium;
        ConcurrentQueue<IJobSystem::Job> high;
        ConcurrentQueue<IJobSystem::Job> critical;
        std::mutex mutex;
        std::condition_variable cv;
    };

    class DefaultJobSystem : public IJobSystem {
    public:
        DefaultJobSystem();
        ~DefaultJobSystem();

        auto name() -> std::string_view override;

        auto thread_count() -> size_t override;
        auto add_job(EJobPriority priority, Job&& job) -> void override;
        auto destroy() -> void override;
    private:
        std::vector<std::unique_ptr<DefaultJobQueue>> m_Queues;
        size_t m_NextQueue;
    };

} // namespace aby::rhi
```


