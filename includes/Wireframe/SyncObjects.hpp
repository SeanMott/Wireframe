#pragma once

//defines fences and semaphores

#include <Wireframe/Core/GPU.hpp>

namespace Wireframe::SyncObjects
{
	//creates a fence
	inline bool Fence_Create(VkFence& fence, Device::GPU* GPU, VkFenceCreateFlags flags = VK_FENCE_CREATE_SIGNALED_BIT)
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.pNext = nullptr;

		info.flags = flags;

		VK_CHECK(vkCreateFence(GPU->device, &info, nullptr, &fence));

		return true;
	}

	//destroys a fence
	inline void Fence_Destroy(VkFence& fence, Device::GPU* GPU)
	{
		vkDestroyFence(GPU->device, fence, nullptr);
	}

	//creates a semaphore
	inline bool Semaphore_Create(VkSemaphore& semaphore, Device::GPU* GPU)
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;

		VK_CHECK(vkCreateSemaphore(GPU->device, &info, nullptr, &semaphore));

		return true;
	}

	//destroys a semaphore
	inline void Semaphore_Destroy(VkSemaphore& semaphore, Device::GPU* GPU)
	{
		vkDestroySemaphore(GPU->device, semaphore, nullptr);
	}
}