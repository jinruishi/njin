#include "vulkan/RenderPass.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "vulkan/Swapchain.h"

namespace {
    using namespace njin::vulkan;

    /**
     * Make a VkRenderPassCreateInfo
     * @param attachment_descriptions Attachment description list
     * @param subpass_descriptions Subpass description list
     * @param subpass_dependencies
     * @return VkRenderPassCreateInfo
     */
    VkRenderPassCreateInfo2 make_render_pass_create_info(
    const std::vector<VkAttachmentDescription2>& attachment_descriptions,
    const std::vector<VkSubpassDescription2>& subpass_descriptions,
    const std::vector<VkSubpassDependency2>& subpass_dependencies);

    VkRenderPassCreateInfo2 make_render_pass_create_info(
    const std::vector<VkAttachmentDescription2>& attachment_descriptions,
    const std::vector<VkSubpassDescription2>& subpass_descriptions,
    const std::vector<VkSubpassDependency2>& subpass_dependencies) {
        VkRenderPassCreateInfo2 info{};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2;
        info.pNext = nullptr;
        info.flags = 0;
        info.attachmentCount = attachment_descriptions.size();
        info.pAttachments = attachment_descriptions.data();
        info.subpassCount = subpass_descriptions.size();
        info.pSubpasses = subpass_descriptions.data();
        info.dependencyCount = subpass_dependencies.size();
        info.pDependencies = subpass_dependencies.data();
        info.correlatedViewMaskCount = 0;
        info.pCorrelatedViewMasks = nullptr;

        return info;
    }
}  // namespace

namespace njin::vulkan {
    RenderPass::RenderPass(const LogicalDevice& device,
                           const Swapchain& swapchain) :
        device_{ device.get() } {
        std::vector<VkAttachmentDescription2> attachment_descriptions{};

        // Color attachment description
        VkAttachmentDescription2 color_desc{};
        color_desc.sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2;
        color_desc.pNext = nullptr;
        color_desc.flags = 0;
        color_desc.format = swapchain.get_image_format();
        color_desc.samples = VK_SAMPLE_COUNT_1_BIT;
        color_desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachment_descriptions.push_back(color_desc);  // index 0

        // Color attachment ref
        VkAttachmentReference2 color_ref{};
        color_ref.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;
        color_ref.pNext = nullptr;
        color_ref.attachment = 0;
        color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        color_ref.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        std::vector<VkSubpassDescription2> subpass_descriptions{};

        // Subpass description (only 1)
        std::vector<VkAttachmentReference2> color_attachment_refs{};
        color_attachment_refs.push_back(color_ref);

        VkSubpassDescription2 subpass_desc{};
        subpass_desc.sType = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2;
        subpass_desc.pNext = nullptr;
        subpass_desc.flags = 0;
        subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_desc.viewMask = 0;
        subpass_desc.inputAttachmentCount = 0;
        subpass_desc.pInputAttachments = nullptr;
        subpass_desc.colorAttachmentCount = color_attachment_refs.size();
        subpass_desc.pColorAttachments = color_attachment_refs.data();
        subpass_desc.pResolveAttachments = nullptr;
        subpass_desc.pDepthStencilAttachment = nullptr;
        subpass_desc.preserveAttachmentCount = 0;
        subpass_descriptions.push_back(subpass_desc);

        // Subpass dependencies
        std::vector<VkSubpassDependency2> subpass_dependencies{};
        VkSubpassDependency2 dependency{
            .sType = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2,
            .pNext = nullptr,
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT,
            .dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT,
            .dependencyFlags = 0,
            .viewOffset = 0
        };
        subpass_dependencies.push_back(dependency);

        VkRenderPassCreateInfo2 render_pass_create_info{
            make_render_pass_create_info(attachment_descriptions,
                                         subpass_descriptions,
                                         subpass_dependencies)
        };

        if (vkCreateRenderPass2(device_,
                                &render_pass_create_info,
                                nullptr,
                                &render_pass_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    RenderPass::RenderPass(const LogicalDevice& device,
                           const VkRenderPassCreateInfo2& create_info) :
        device_{ device.get() } {
        if (vkCreateRenderPass2(device_,
                                &create_info,
                                nullptr,
                                &render_pass_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    RenderPass::~RenderPass() {
        vkDestroyRenderPass(device_, render_pass_, nullptr);
    }

    VkRenderPass RenderPass::get() const {
        return render_pass_;
    }
}  // namespace njin::vulkan
