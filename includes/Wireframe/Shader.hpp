#pragma once

//defines shaders

#include <BTDSTD/Logger.hpp>

#include <Wireframe/Core/GPU.hpp>

#include <fstream>
#include <vector>

namespace Wireframe::Shader
{
	//defines a shader module
	struct ShaderModule
	{
		VkShaderModule shaderModule;

		//creates a shader module
		inline bool Create(const char* filepath, Device::GPU* GPU)
		{
			//open the file. With cursor at the end
			std::ifstream file(filepath, std::ios::ate | std::ios::binary);

			if (!file.is_open())
			{
				fmt::print("Wireframe Error: Shader Module || Create || Failed to open a compiled shader at \"{}\". Please make sure the path actually exists\n", filepath);
				return false;
			}

			//find what the size of the file is by looking up the location of the cursor
			//because the cursor is at the end, it gives the size directly in bytes
			size_t fileSize = (size_t)file.tellg();

			//spirv expects the buffer to be on uint32, so make sure to reserve a int vector big enough for the entire file
			std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

			//put file cursor at beggining
			file.seekg(0);

			//load the entire file into the buffer
			file.read((char*)buffer.data(), fileSize);

			//now that the file is loaded into the buffer, we can close it
			file.close();

			//create a new shader module, using the buffer we loaded
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pNext = nullptr;

			//codeSize has to be in bytes, so multply the ints in the buffer by size of int to know the real size of the buffer
			createInfo.codeSize = buffer.size() * sizeof(uint32_t);
			createInfo.pCode = buffer.data();

			//check that the creation goes well.
			if (vkCreateShaderModule(GPU->device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			{
				fmt::print("Wireframe Error: Shader Module || Create || Failed to create a shader from Byte Code loaded at \"{}\". Please make sure the path actually exists and is a compiled SPRV shader. The BTDSTD comes with tools for compiling shaders. If you are missing parts of the SDK or have a question go to {}\n",
					filepath, "\"www.bytesthedust.co\"");
				return false;
			}

			return true;
		}

		//destroys the module
		inline void Destroy(Device::GPU* GPU)
		{
			vkDestroyShaderModule(GPU->device, shaderModule, nullptr);
		}
	};

	//generates a vertex shader stage
	static inline VkPipelineShaderStageCreateInfo GenerateVertexShaderStage(const ShaderModule& shader, const char* entryPointFuncName = "main")
	{
		VkPipelineShaderStageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.pNext = nullptr;

		//shader stage
		info.stage = VK_SHADER_STAGE_VERTEX_BIT;
		//module containing the code for this shader stage
		info.module = shader.shaderModule;
		//the entry point of the shader
		info.pName = entryPointFuncName;

		return info;
	}

	//generates a fragment shader stage
	static inline VkPipelineShaderStageCreateInfo GenerateFragmentShaderStage(const ShaderModule& shader, const char* entryPointFuncName = "main")
	{
		VkPipelineShaderStageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.pNext = nullptr;

		//shader stage
		info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		//module containing the code for this shader stage
		info.module = shader.shaderModule;
		//the entry point of the shader
		info.pName = entryPointFuncName;

		return info;
	}
}