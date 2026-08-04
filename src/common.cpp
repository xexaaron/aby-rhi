#include "common.hpp"

#include "context.hpp"

namespace aby::rhi::detail {

	ScopedTimer::ScopedTimer(std::string_view name) : m_Name(name),
	                                                  m_Start(Clock::now()) {
	}

	ScopedTimer::~ScopedTimer() {
		const auto end     = Clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - m_Start);

		aby_rhi_dbg("[timer] {} took {}us", m_Name.data(), static_cast<unsigned long long>(elapsed.count()));
	}

} // namespace aby::rhi::detail
