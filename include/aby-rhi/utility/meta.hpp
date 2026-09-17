#pragma once
#include <cstddef>

namespace aby::rhi::meta {

	template <typename>
	struct member_pointer_traits;

	template <typename Class, typename Member>
	struct member_pointer_traits<Member Class::*> {
		using class_type  = Class;
		using member_type = Member;
	};

	/**
	* @brief Get the offset of a struct/class member
	* @tparam T the object type
	* @tparam Member the member in the format &T::member
	* @return offset of in bytes
	*/
	template <typename T, auto Member>
	static size_t offset_of() {
		return reinterpret_cast<size_t>(&(reinterpret_cast<volatile const T*>(0)->*Member));
	}

} // namespace aby::rhi::meta
