#pragma once

//defines operations for command buffers and command pools

#include <Wireframe/Core/GPU.hpp>

namespace Wireframe::CommandBuffer
{
	//defines a command buffer
	struct CommandBuffer
	{
		VkCommandBuffer handle;
		VkCommandBufferLevel kind = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		bool isRecording = false;

		//starts recording
		inline bool StartRecording()
		{
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.pNext = nullptr;

			info.pInheritanceInfo = nullptr;
			info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			VK_CHECK(vkBeginCommandBuffer(handle, &info));

			isRecording = true;
			return true;
		}

		//stops recording
		inline void EndRecording()
		{
			VK_CHECK(vkEndCommandBuffer(handle));
			isRecording = false;
		}

		//resets a command buffer
		inline void Reset()
		{
			VK_CHECK(vkResetCommandBuffer(handle, 0));
		}
	};

	//create info for a command pool
	struct CommandPool_CreateInfo
	{
		bool canBeReset = true; //VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT

		uint32_t queueFamilyIndex = 0; //used for compute or rendering
	};

	//defines a command pool
	struct CommandPool
	{
		VkCommandPool handle;

		std::vector<CommandBuffer> commandBuffers;

		//creates a command pool
		inline bool Create(CommandPool_CreateInfo& _info, Device::GPU* GPU)
		{
			VkCommandPoolCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.pNext = nullptr;

			info.flags = ((_info.canBeReset == true ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0));
			info.queueFamilyIndex = _info.queueFamilyIndex;

			VK_CHECK(vkCreateCommandPool(GPU->device, &info, nullptr, &handle));

			commandBuffers.reserve(2); //reserve space for at least 2 command buffers

			return true;
		}

		//destroys a command pool
		inline void Destroy(Device::GPU* GPU)
		{
			vkDestroyCommandPool(GPU->device, handle, nullptr);
		}

		//allocates command buffers
		inline bool AllocateCommandBuffers(const size_t& amount, const VkCommandBufferLevel& kind, Device::GPU* GPU)
		{
			VkCommandBufferAllocateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			info.pNext = nullptr;

			info.commandPool = handle;
			info.commandBufferCount = amount;
			info.level = kind;

			//do we already have command buffers allocated in this pool
			const size_t alreadyExistingCmdBuffersCount = commandBuffers.size();
			bool doBuffersAlreadyExist = (alreadyExistingCmdBuffersCount > 0 ? true : false);

			//are we just allocating a single buffer
			bool moreThenOneNewBuffer = (amount > 1 ? true : false);

			//if it's a single buffer
			if (!moreThenOneNewBuffer)
			{
				CommandBuffer* cmd = nullptr;

				//if we already have buffers, just emplace a new one place
				if (doBuffersAlreadyExist)
					cmd = &commandBuffers.emplace_back(CommandBuffer());

				//if we don't already have buffers, we can just resize
				else
				{
					commandBuffers.resize(1);
					cmd = &commandBuffers[0];
				}

				VK_CHECK(vkAllocateCommandBuffers(GPU->device, &info, &cmd->handle));
				cmd->kind = kind;
			}

			//if it's more then one
			else
			{
				//if they don't already exist, we can just resize, allocate and go
				if (!doBuffersAlreadyExist)
				{
					commandBuffers.resize(amount);
					std::vector<VkCommandBuffer> cmds; cmds.resize(amount);
					VK_CHECK(vkAllocateCommandBuffers(GPU->device, &info, cmds.data()));
					for (size_t i = 0; i < amount; ++i)
					{
						commandBuffers[i].handle = cmds[i];
						commandBuffers[i].kind = kind;
					}
				}

				//if we already have buffers we gotta offset em
				else
				{
					std::vector<VkCommandBuffer> cmds; cmds.resize(amount);
					VK_CHECK(vkAllocateCommandBuffers(GPU->device, &info, cmds.data()));
					for (size_t i = 0; i < amount; ++i)
					{
						commandBuffers.emplace_back(CommandBuffer());
						CommandBuffer* cmd = &commandBuffers[alreadyExistingCmdBuffersCount + i];
						cmd->handle = cmds[i];
						cmd->kind = kind;
					}
				}
			}

			return true;
		}
	};
}