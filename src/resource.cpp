#include "resource.hpp"

namespace aby::rhi {

	Resource::Resource(EResource type, ResourceID id) :
	    m_Type(type),
	    m_ID(id) {
	}

	auto Resource::type() const -> EResource {
		return m_Type;
	}

	auto Resource::id() const -> ResourceID {
		return m_ID;
	}

	Resource::operator ResourceID() const {
		return m_ID;
	}

	Resource::operator std::pair<EResource, ResourceID>() const {
		return std::make_pair(m_Type, m_ID);
	}

	Resource::operator bool() const {
		return m_ID != ResourceID::invalid;
	}

	auto Resource::operator==(Resource other) const -> bool {
		return m_Type == other.m_Type && m_ID == other.m_ID;
	}

} // namespace aby::rhi
