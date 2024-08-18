#pragma once

//defines structs and functions for subpasses and deps

#include <Wireframe/Util/Attachment.hpp>

#include <vector>

namespace Wireframe::Util::Subpass
{
    //defines a subpass dependicy
    struct SubpassDependency
    {
        uint32_t                srcSubpass = VK_SUBPASS_EXTERNAL;
        uint32_t                dstSubpass = 0;
        VkPipelineStageFlags    srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkPipelineStageFlags    dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkAccessFlags           srcAccessMask = 0;
        VkAccessFlags           dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        VkDependencyFlags       dependencyFlags = 0;

        //generates a vulkan subpass dependicy
        inline VkSubpassDependency GenerateVulkanSubpassDependency() const
        {
            VkSubpassDependency dep = {};
            dep.srcSubpass = srcSubpass;
            dep.dstSubpass = dstSubpass;
            dep.srcStageMask = srcStageMask;
            dep.dstStageMask = dstStageMask;
            dep.srcAccessMask = srcAccessMask;
            dep.dstAccessMask = dstAccessMask;
            dep.dependencyFlags = dependencyFlags;
            return dep;
        }
    };

    //generate a default subpass dependency for reading and writing color data
    static inline SubpassDependency GenerateDefault_SubpassDependency_ColorReadAndWrite()
    {
        Wireframe::Util::Subpass::SubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        return dependency;
    }

    //generate a default depth pass subpass dependency dependent on the previous render pass's depth data
    static inline SubpassDependency GenerateDefault_SubpassDependency_DepthWriteData()
    {
        Wireframe::Util::Subpass::SubpassDependency depthDependency = {};
        depthDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        depthDependency.dstSubpass = 0;
        depthDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        depthDependency.srcAccessMask = 0;
        depthDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        depthDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        return depthDependency;
    }

    //defines a subpass
	struct Subpass
	{
        bool isDirty = true; //is this description dirty and needs to be regenerated

        VkPipelineBindPoint             pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        VkSubpassDescriptionFlags       flags = 0;

        std::vector<Attachment::Attachment> colorAttachments;
        std::vector<Attachment::Attachment> depthStencilAttachments;
        //input attachments

        //attachments to perserve

    private:

        //the raw vulkan structs we use internally
        std::vector<VkAttachmentReference> colorAttachments_rawVulkanRefs;
        std::vector<VkAttachmentReference> depthStencilAttachments_rawVulkanRefs;

    public:

        //adds a color attachment

        //removes a color attachment

        //adds a depth attachment

        //removes a color attachment

        //generates a vulkan subpass
        inline VkSubpassDescription GenerateVulkanSubpassDescription()
        {
            //if it's dirty regenerate the internal vulkan structs
            if (isDirty)
            {
                const size_t colorRes = colorAttachments.size();
                const size_t depthRes = depthStencilAttachments.size();
                colorAttachments_rawVulkanRefs.clear(); colorAttachments_rawVulkanRefs.resize(colorRes);
                depthStencilAttachments_rawVulkanRefs.clear(); depthStencilAttachments_rawVulkanRefs.resize(depthRes);

                for (size_t i = 0; i < colorRes; ++i)
                    colorAttachments_rawVulkanRefs[i] = colorAttachments[i].GenerateVulkanAttachmentRef();
                for (size_t i = 0; i < depthRes; ++i)
                    depthStencilAttachments_rawVulkanRefs[i] = depthStencilAttachments[i].GenerateVulkanAttachmentRef();

                isDirty = false;
            }

            VkSubpassDescription subpass = {};
            subpass.flags = flags;
            subpass.pipelineBindPoint = pipelineBindPoint;
            
            subpass.colorAttachmentCount = colorAttachments.size();
            subpass.pColorAttachments = colorAttachments_rawVulkanRefs.data();

            subpass.pDepthStencilAttachment = depthStencilAttachments_rawVulkanRefs.data();

            //for now hard coded
            subpass.inputAttachmentCount = 0;
            subpass.pInputAttachments = nullptr;
            subpass.pResolveAttachments = nullptr;
            subpass.preserveAttachmentCount = 0;
            subpass.pPreserveAttachments = nullptr;

            return subpass;
        }
	};
}