#pragma once

//defines a pipeline layout

#include <Wireframe/Core/GPU.hpp>

#include <unordered_map>

namespace Wireframe::Pipeline
{
	//defines a pipeline layout create info
	struct PipelineLayout_CreateInfo
	{
		std::vector<VkPushConstantRange> pushConstants;
		std::vector<std::string> pushConstantNames;
	};

	//defines a pipeline layout
	struct PipelineLayout
	{
		std::unordered_map<std::string, size_t> pushConstants; //the push constants associated with this layout

		VkPipelineLayout handle;

		//creates a pipeline layout
		inline bool Create(const PipelineLayout_CreateInfo& _info, Device::GPU* GPU)
		{
			VkPipelineLayoutCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			info.pNext = nullptr;

			info.flags = 0;

			info.setLayoutCount = 0;
			info.pSetLayouts = nullptr;
			
			const size_t pushConstCount = _info.pushConstants.size();
			if (pushConstCount > 0)
			{
				info.pushConstantRangeCount = _info.pushConstants.size();
				info.pPushConstantRanges = _info.pushConstants.data();
				for (size_t i = 0; i < pushConstCount; ++i)
					pushConstants[_info.pushConstantNames[i]] = _info.pushConstants[i].size;
			}
			else
			{
				info.pushConstantRangeCount = 0;
				info.pPushConstantRanges = 0;
			}

			VK_CHECK(vkCreatePipelineLayout(GPU->device, &info, nullptr, &handle));

			return true;
		}

		//destroys a pipeline layout
		inline void Destroy(Device::GPU* GPU)
		{
			vkDestroyPipelineLayout(GPU->device, handle, nullptr);
		}

		//update push constant on the vertex stage
		inline void UpdatePushConstant_Vertex(VkCommandBuffer& cmd, const std::string pushConstantName, void* data)
		{
			vkCmdPushConstants(cmd, handle, VK_SHADER_STAGE_VERTEX_BIT, 0, pushConstants[pushConstantName], data);
		}
	};
}