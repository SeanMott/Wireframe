// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <Wireframe/vk_types.h>

namespace vkinit {

	VkSubmitInfo submit_info(VkCommandBuffer* cmd);

	VkPresentInfoKHR present_info();

	VkImageCreateInfo image_create_info(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);

	VkImageViewCreateInfo imageview_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

	VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
}

