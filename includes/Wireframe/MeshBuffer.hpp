#pragma once

//defines mesh buffers for handling drawing data

#include <Wireframe/vk_types.h>

namespace Wireframe::MeshBuffers
{
	//single mesh vertex buffer
	struct VertexBuffer
	{
		size_t vertexCount = 0;

		VkBuffer buffer;
		VmaAllocation allocation;

		//creates a vertex buffer
		inline bool Create(VmaAllocator& allocator, const size_t& sizeofSingleVertex,
			const void* vertexData, const size_t& _vertexCount)
		{
			vertexCount = _vertexCount;

			//allocate vertex buffer
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			bufferInfo.size = vertexCount * sizeofSingleVertex;
			//this buffer is going to be used as a Vertex Buffer
			bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

			//allocate the buffer
			VK_CHECK(vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
				&buffer,
				&allocation,
				nullptr));

			//copy vertex data
			void* data;
			vmaMapMemory(allocator, allocation, &data);

			memcpy(data, vertexData, bufferInfo.size);

			vmaUnmapMemory(allocator, allocation);

			return true;
		}

		//destroys the buffer
		inline void Destroy(VmaAllocator& allocator) { vmaDestroyBuffer(allocator, buffer, allocation); }

		//binds the buffer
		inline void Bind(VkCommandBuffer& cmd)
		{
			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(cmd, 0, 1, &buffer, &offset);
		}

		//binds the buffer with a offset

		//draws the entire buffer
		inline void Draw(VkCommandBuffer& cmd, uint32_t instanceCount, uint32_t currentInstance)
		{
			vkCmdDraw(cmd, vertexCount, instanceCount, 0, currentInstance);
		}

		//draws with a starting index into the buffer
	};

	//single mesh index buffer
	struct IndexBuffer
	{
		size_t indicesCount = 0;

		VkBuffer buffer;
		VmaAllocation allocation;

		//creates a indices buffer
		inline bool Create(VmaAllocator& allocator,
			const uint32_t* indicesData, const size_t& _indicesCount)
		{
			indicesCount = _indicesCount;

			//allocate index buffer
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			bufferInfo.size = indicesCount * sizeof(uint32_t);
			//this buffer is going to be used as a Index Buffer
			bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

			//allocate the buffer
			VK_CHECK(vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
				&buffer,
				&allocation,
				nullptr));

			//copy vertex data
			void* data;
			vmaMapMemory(allocator, allocation, &data);

			memcpy(data, indicesData, bufferInfo.size);

			vmaUnmapMemory(allocator, allocation);

			return true;
		}

		//destroys the buffer
		inline void Destroy(VmaAllocator& allocator) { vmaDestroyBuffer(allocator, buffer, allocation); }

		//binds the buffer
		inline void Bind(VkCommandBuffer& cmd)
		{
			VkDeviceSize offset = 0;
			vkCmdBindIndexBuffer(cmd, buffer, offset, VK_INDEX_TYPE_UINT32);
		}

		//binds the buffer with a offset

		//draws the entire buffer
		inline void Draw(VkCommandBuffer& cmd, uint32_t instanceCount, uint32_t currentInstance)
		{
			vkCmdDrawIndexed(cmd, indicesCount, instanceCount, 0, 0, currentInstance);
		}

		//draws with a starting index into the buffer
	};

	//multi-mesh vertex buffer

	//multi-mesh index buffer

	//mult-mesh interleafed mesh and index buffer
}