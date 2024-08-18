#pragma once

//defines various attachments

#include <Wireframe/vk_initializers.h>

namespace Wireframe::Util::Attachment
{
	//defines a attachment
	struct Attachment
	{
        VkAttachmentDescriptionFlags    flags = 0;
        VkFormat                        format = VK_FORMAT_UNDEFINED;
        VkSampleCountFlagBits           samples = VK_SAMPLE_COUNT_1_BIT;
        VkAttachmentLoadOp              loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        VkAttachmentStoreOp             storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        VkAttachmentLoadOp              stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        VkAttachmentStoreOp             stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        VkImageLayout                   initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageLayout                   finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        uint32_t         attachmentIndex = 0; //the index
        VkImageLayout    attachmentImageKind = VK_IMAGE_LAYOUT_UNDEFINED; //kind of attachment/image it is assocated with

        //generates a Vulkan attachment
        inline VkAttachmentDescription GenerateVulkanAttachmentDescription() const
        {
            VkAttachmentDescription attachment = {};
            attachment.flags = flags;
            attachment.format = format;
            attachment.samples = samples;
            attachment.loadOp = loadOp;
            attachment.storeOp = storeOp;
            attachment.stencilLoadOp = stencilLoadOp;
            attachment.stencilStoreOp = stencilStoreOp;
            attachment.initialLayout = initialLayout;
            attachment.finalLayout = finalLayout;
            return attachment;
        }

        //generates a vulkan attachment ref
        inline VkAttachmentReference GenerateVulkanAttachmentRef() const
        {
            VkAttachmentReference ref = {};
            ref.attachment = attachmentIndex;
            ref.layout = attachmentImageKind;
            return ref;
        }
	};

    //generates default parameters for color attachment
    static inline Attachment GenerateDefaultAttachment_Color(const VkFormat& swapchainFormat)
    {
        Wireframe::Util::Attachment::Attachment colorAttachment;
        colorAttachment.flags = 0;
        colorAttachment.format = swapchainFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        colorAttachment.attachmentIndex = 0;
        colorAttachment.attachmentImageKind = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        return colorAttachment;
    }

    //generates default parameters for a depth stencil attachment
    static inline Attachment GenerateDefaultAttachment_DepthStencil(const VkFormat& swapchainDepthFormat)
    {
        Wireframe::Util::Attachment::Attachment depthAttachment;
        depthAttachment.flags = 0;
        depthAttachment.format = swapchainDepthFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.attachmentIndex = 1;
        depthAttachment.attachmentImageKind = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        return depthAttachment;
    }
}