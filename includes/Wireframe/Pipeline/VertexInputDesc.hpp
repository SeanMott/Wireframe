#pragma once

//defines vertex input description

#include <vulkan/vulkan.h>
#include <vector>

namespace Wireframe::Pipeline
{
	//defines a vertex input description
	struct VertexInputDescription
	{
		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkVertexInputAttributeDescription> attributes;

		VkPipelineVertexInputStateCreateFlags flags = 0;
	};
}