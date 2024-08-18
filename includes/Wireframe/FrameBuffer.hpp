#pragma once

//manages frame buffers

#include <Wireframe/Core/GPU.hpp>

namespace Wireframe::FrameBuffer
{
	//creates a frame buffer(s)
	inline bool FrameBufferCreate(std::vector<VkFramebuffer>& frameBuffers, const size_t& createCount, const VkImageView* views, VkImageView depthView,
		VkRenderPass renderPass, VkExtent2D size, Device::GPU* GPU)
	{
		frameBuffers.resize(createCount);

		VkFramebufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.pNext = nullptr;

		info.renderPass = renderPass;
		
		info.width = size.width;
		info.height = size.height;
		info.layers = 1;

		info.attachmentCount = 2;
		VkImageView attachments[2];
		attachments[1] = depthView;
		info.pAttachments = attachments;

		//if for some reason only one frame buffer is being created
		if (createCount == 1)
		{
			attachments[0] = views[0];
			VK_CHECK(vkCreateFramebuffer(GPU->device, &info, nullptr, &frameBuffers[0]));
		}

		//if it's multiable
		else
		{
			for (uint32_t i = 0; i < createCount; i++)
			{
				attachments[0] = views[i];
				VK_CHECK(vkCreateFramebuffer(GPU->device, &info, nullptr, &frameBuffers[i]));
			}
		}
		return true;
	}

	//destroys a frame buffer(s)
	inline void DestroyFrameBuffers(std::vector<VkFramebuffer>& frameBuffers, Device::GPU* GPU)
	{
		const register size_t frameBufferCount = frameBuffers.size();

		//if just one for some reason
		if(frameBufferCount == 1)
			vkDestroyFramebuffer(GPU->device, frameBuffers[0], nullptr);

		//if multiable
		else
		{
			for (int i = frameBufferCount - 1; i > -1; i--)
				vkDestroyFramebuffer(GPU->device, frameBuffers[i], nullptr);
		}
	}
}