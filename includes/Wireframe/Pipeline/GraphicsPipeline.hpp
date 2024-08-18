#pragma once

//defines a graphics pipeline

#include <Wireframe/Pipeline/PipelineSettings.hpp>
#include <Wireframe/Pipeline/PipelineLayout.hpp>

namespace Wireframe::Pipeline
{
	//defines a graphics pipeline
	struct GraphicsPipeline
	{
		VkPipeline handle;
		PipelineLayout layout;

		PipelineSettings settings;

		//creates a graphics pipeline
		inline bool Create(PipelineSettings& _settings, PipelineLayout& _layout, VkRenderPass& renderpass, Device::GPU* GPU)
		{
			settings = _settings;
			layout = _layout;

			//make viewport state from our stored viewport and scissor.
			//at the moment we wont support multiple viewports or scissors
			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.pNext = nullptr;

			viewportState.viewportCount = 1;
			viewportState.pViewports = &settings._viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &settings._scissor;

			//setup dummy color blending. We arent using transparent objects yet
			//the blending is just "no blend", but we do write to the color attachment
			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.pNext = nullptr;

			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &settings._colorBlendAttachment;

			//build the actual pipeline
			//we now use all of the info structs we have been writing into into this one to create the pipeline
			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.pNext = nullptr;

			pipelineInfo.stageCount = settings._shaderStages.size();
			pipelineInfo.pStages = settings._shaderStages.data();
			pipelineInfo.pVertexInputState = &settings._vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &settings._inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &settings._rasterizer;
			pipelineInfo.pMultisampleState = &settings._multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.pDepthStencilState = &settings._depthStencil;
			pipelineInfo.layout = layout.handle;
			pipelineInfo.renderPass = renderpass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			//its easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
			if (vkCreateGraphicsPipelines(
				GPU->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &handle) != VK_SUCCESS) {
				std::cout << "failed to create pipline\n";
				return false; // failed to create graphics pipeline
			}
			//else
			//{
			//	return newPipeline;
			//}

			return true;
		}

		//destroys the pipeline
		inline void Destroy(Device::GPU* GPU)
		{
			vkDestroyPipeline(GPU->device, handle, nullptr);
		}
	};
}