#include "vulkan/Framebuffer.h"

#include <stdexcept>

namespace njin::vulkan {
    Framebuffer::Framebuffer(const LogicalDevice& device,
                             const RenderPass& render_pass,
                             const ImageView& image_view) : device_{
            device.get()
        },
        extent_{ image_view.get_extent() } {
        VkFramebufferCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.renderPass = render_pass.get();

        std::vector<VkImageView> attachments{ image_view.get() };
        info.attachmentCount = attachments.size();
        info.pAttachments = attachments.data();
        info.width = image_view.get_extent().width;
        info.height = image_view.get_extent().height;
        info.layers = 1;

        if (vkCreateFramebuffer(device_, &info, nullptr, &framebuffer_) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        };
    }

    Framebuffer::~Framebuffer() {
        vkDestroyFramebuffer(device_, framebuffer_, nullptr);
    }

    VkFramebuffer Framebuffer::get() const {
        return framebuffer_;
    }

    VkExtent2D Framebuffer::get_extent() const {
        return extent_;
    };
}
