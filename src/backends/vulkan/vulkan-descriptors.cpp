#include "backends/vulkan/vulkan-descriptors.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

namespace aby::rhi::vulkan {

	auto DescriptorLayoutBuilder::add_binding(uint32_t binding, vk::DescriptorType type) -> DescriptorLayoutBuilder& {
		vk::DescriptorSetLayoutBinding new_binding(binding, type, 1);
		m_Bindings.push_back(new_binding);
		return *this;
	}

	auto DescriptorLayoutBuilder::set_flags(vk::DescriptorSetLayoutCreateFlags flags) -> DescriptorLayoutBuilder& {
		m_DescriptorFlags = flags;
		return *this;
	}

	auto DescriptorLayoutBuilder::set_pnext(void* pnext) -> DescriptorLayoutBuilder& {
		m_pNext = pnext;
		return *this;
	}

	auto DescriptorLayoutBuilder::set_stages(vk::ShaderStageFlags flags) -> DescriptorLayoutBuilder& {
		m_StageFlags = flags;
		return *this;
	}

	auto DescriptorLayoutBuilder::clear() -> void {
		m_Bindings.clear();
	}

	auto DescriptorLayoutBuilder::build() -> vk::DescriptorSetLayout {
		for (auto& b : m_Bindings) {
			b.setStageFlags(m_StageFlags);
		}

		vk::DescriptorSetLayoutCreateInfo create_info(
		    m_DescriptorFlags,
		    m_Bindings.size(),
		    m_Bindings.data(),
		    m_pNext);

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vk::DescriptorSetLayout layout;
		vkassert(vkCreateDescriptorSetLayout(
		             r->device(),
		             vkcast(create_info),
		             allocator(),
		             vkcast(layout)),
		         "failed to create descriptor set layout");

		return layout;
	}

} // namespace aby::rhi::vulkan

namespace aby::rhi::vulkan {

	auto DescriptorAllocator::init(uint32_t max_sets, std::span<PoolSizeRatio> pool_ratios) -> bool {
		std::vector<vk::DescriptorPoolSize> pool_sizes;
		pool_sizes.resize(pool_ratios.size());

		for (size_t i = 0; i < pool_ratios.size(); i++) {
			pool_sizes[i].type            = pool_ratios[i].type;
			pool_sizes[i].descriptorCount = pool_ratios[i].ratio * max_sets;
		}

		vk::DescriptorPoolCreateInfo create_info(
		    vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
		    max_sets,
		    pool_sizes.size(),
		    pool_sizes.data());

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vkcheck(vkCreateDescriptorPool(
		            r->device(),
		            vkcast(create_info),
		            allocator(),
		            vkcast(m_Pool)),
		        "failed to create descriptor pool");

		return true;
	}

	auto DescriptorAllocator::alloc(vk::DescriptorSetLayout layout) -> vk::DescriptorSet {
		vk::DescriptorSetAllocateInfo alloc_info(
		    m_Pool,
		    1, /* descriptor set count */
		    &layout);

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vk::DescriptorSet ds;
		vkassert(vkAllocateDescriptorSets(
		             r->device(),
		             vkcast(alloc_info),
		             vkcast(ds)),
		         "failed to allocate descriptor set(s)");

		return ds;
	}

	auto DescriptorAllocator::clear() -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vkcheck(vkResetDescriptorPool(
		            r->device(),
		            m_Pool,
		            0),
		        "failed to reset descriptor pool");

		return true;
	}

	auto DescriptorAllocator::deinit() -> void {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vkDestroyDescriptorPool(
		    r->device(),
		    m_Pool,
		    allocator());
	}

} // namespace aby::rhi::vulkan
