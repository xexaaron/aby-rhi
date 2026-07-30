#include "backends/vulkan/vulkan-helpers.hpp"
#include "context.hpp"

namespace aby::rhi::vulkan {

    auto transition_image(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout src, vk::ImageLayout dst) -> void {
        vk::ImageMemoryBarrier2 image_barrier(
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite,
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
            src, dst,
            0, 0,   
            image,
            vk::ImageSubresourceRange(
                dst == vk::ImageLayout::eDepthAttachmentOptimal ?
                    vk::ImageAspectFlagBits::eDepth             :
                    vk::ImageAspectFlagBits::eColor,
                0,
                VK_REMAINING_MIP_LEVELS,
                0,
                VK_REMAINING_ARRAY_LAYERS
            )
        );

        vk::DependencyInfo dep_info({}, 0, nullptr, 0, nullptr, 1, &image_barrier);

        vkCmdPipelineBarrier2(
            cmd,
            reinterpret_cast<VkDependencyInfo*>(&dep_info)
        );
    }

    auto copy_image_to_image(vk::CommandBuffer cmd, vk::Image src, vk::Extent2D src_sz, vk::Image dst, vk::Extent2D dst_sz) -> void {
        VkImageBlit2 blit_region{ .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr };

        blit_region.srcOffsets[1].x = src_sz.width;
        blit_region.srcOffsets[1].y = src_sz.height;
        blit_region.srcOffsets[1].z = 1;
        
        blit_region.dstOffsets[1].x = dst_sz.width;
        blit_region.dstOffsets[1].y = dst_sz.height;
        blit_region.dstOffsets[1].z = 1;

        blit_region.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit_region.srcSubresource.baseArrayLayer = 0;
        blit_region.srcSubresource.layerCount     = 1;
        blit_region.srcSubresource.mipLevel       = 0;
        blit_region.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit_region.dstSubresource.baseArrayLayer = 0;
        blit_region.dstSubresource.layerCount     = 1;
        blit_region.dstSubresource.mipLevel       = 0;

        VkBlitImageInfo2 blit_info{ .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
        blit_info.dstImage       = dst;
        blit_info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        blit_info.srcImage       = src;
        blit_info.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        blit_info.filter         = VK_FILTER_LINEAR;
        blit_info.regionCount    = 1;
        blit_info.pRegions       = &blit_region;

        vkCmdBlitImage2(cmd, &blit_info);
    }


    auto clear_screen(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout layout, vk::ClearColorValue& color, vk::ImageSubresourceRange& range) -> void {
        vkCmdClearColorImage(
            cmd,
            image,
            static_cast<VkImageLayout>(layout),
            reinterpret_cast<VkClearColorValue*>(&color),
            1,
            reinterpret_cast<VkImageSubresourceRange*>(&range)
        );
    }



    auto log_error(const std::string& message, const vkb::Error& error) -> void {
        aby_rhi_err("[vulkan] {}", message);
        for (size_t i = 0; i < error.detailed_failure_reasons.size(); i++) {
            aby_rhi_err("[vulkan] {}) {}", i, error.detailed_failure_reasons[i]);
        }
    }

    auto eformat_to_vkformat(EFormat format) -> vk::Format {
        switch (format) {
            case EFormat::none:      return vk::Format::eUndefined;
            case EFormat::rgba_f32:  return vk::Format::eR32G32B32A32Sfloat;
            case EFormat::rgb_f32:   return vk::Format::eR32G32B32Sfloat;
            case EFormat::rg_f32:    return vk::Format::eR32G32Sfloat;
            case EFormat::r_f32:     return vk::Format::eR32Sfloat;
            case EFormat::rgba_i32:  return vk::Format::eR32G32B32A32Sint;
            case EFormat::rgb_i32:   return vk::Format::eR32G32B32Sint;
            case EFormat::rg_i32:    return vk::Format::eR32G32Sint;
            case EFormat::r_i32:     return vk::Format::eR32Sint;
            case EFormat::rgba_u32:  return vk::Format::eR32G32B32A32Uint;
            case EFormat::rgb_u32:   return vk::Format::eR32G32B32Uint;
            case EFormat::rg_u32:    return vk::Format::eR32G32Uint;
            case EFormat::r_u32:     return vk::Format::eR32Uint;
            default:
                aby_rhi_assert(false, "unknown EFormat -> vk::Format conversion");
                return vk::Format::eUndefined;
        }
    }

}