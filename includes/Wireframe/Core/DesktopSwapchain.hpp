#pragma once

//defines a swapchain for desktop

#include <Wireframe/Core/DesktopWindow.hpp>
#include <Wireframe/Core/GPU.hpp>

#include <Wireframe/vk_initializers.h>

#include <VkBootstrap.h>

namespace Wireframe::Swapchain
{
	//defines a desktop swapchain create info
	struct DesktopSwapchain_CreateInfo
	{

	};

	//defines a desktop swapchain
	struct DesktopSwapchain
	{
		//depth resources
		VkImageView _depthImageView;
		AllocatedImage _depthImage;

		//the format for the depth image
		VkFormat _depthFormat;

		VkSwapchainKHR _swapchain;
		VkFormat _swachainImageFormat;

		std::vector<VkImage> _swapchainImages;
		std::vector<VkImageView> _swapchainImageViews;

		//creates a swapchain
		inline bool Create(const DesktopSwapchain_CreateInfo& info,
			Device::GPU* GPU, Window::DesktopWindow* window, VmaAllocator& allocator)
		{
			vkb::SwapchainBuilder swapchainBuilder{ GPU->chosenGPU, GPU->device, window->_surface };

			vkb::Swapchain vkbSwapchain = swapchainBuilder
				.use_default_format_selection()
				//use vsync present mode
				.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
				.set_desired_extent(window->_windowExtent.width, window->_windowExtent.height)
				.build()
				.value();
			if (!vkbSwapchain)
			{
				fmt::print("Wireframe Error: Desktop Swapchain || Create || Failed to create a Swapchain!\n");
				return false;
			}

			//store swapchain and its related images
			_swapchain = vkbSwapchain.swapchain;
			_swapchainImages = vkbSwapchain.get_images().value();
			_swapchainImageViews = vkbSwapchain.get_image_views().value();

			_swachainImageFormat = vkbSwapchain.image_format;

			//depth image size will match the window
			VkExtent3D depthImageExtent = {
				window->_windowExtent.width,
				window->_windowExtent.height,
				1
			};

			//hardcoding the depth format to 32 bit float
			_depthFormat = VK_FORMAT_D32_SFLOAT;

			//the depth image will be a image with the format we selected and Depth Attachment usage flag
			VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

			//for the depth image, we want to allocate it from gpu local memory
			VmaAllocationCreateInfo dimg_allocinfo = {};
			dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
			dimg_allocinfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;// VkMemoryPropertyFlags(VK_MEMORY_PROPERTYGPU.device_LOCAL_BIT);

			//allocate and create the image
			vmaCreateImage(allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

			//build a image-view for the depth image to use for rendering
			VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);;

			VK_CHECK(vkCreateImageView(GPU->device, &dview_info, nullptr, &_depthImageView));

			return true;
		}

		//destroys a swapchain
		inline void Destroy(Device::GPU* GPU, VmaAllocator& allocator)
		{
			vkDestroyImageView(GPU->device, _depthImageView, nullptr);
			vmaDestroyImage(allocator, _depthImage._image, _depthImage._allocation);
			
			for(uint32_t i = 0; i < _swapchainImageViews.size(); ++i)
				vkDestroyImageView(GPU->device, _swapchainImageViews[i], nullptr);
			
			vkDestroySwapchainKHR(GPU->device, _swapchain, nullptr);
		}

		//remakes a swapchain
		inline void Remake()
		{

		}
	};
}