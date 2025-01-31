#pragma once
#include <vulkan/vulkan_core.h>

#include "vulkan/LogicalDevice.h"
#include "vulkan/RenderPass.h"

namespace njin::vulkan {
    class Framebuffer {
    public:
        /**
         * Constructor
         * @param device Device to create this framebuffer on
         * @param render_pass Render pass this framebuffer will be compatible with
         * @param image_view Image view this framebuffer will contain
         */
        Framebuffer(const LogicalDevice& device,
                    const RenderPass& render_pass,
                    const ImageView& image_view);


        Framebuffer(const Framebuffer& other) = delete;

        Framebuffer(Framebuffer&& other) = delete;

        Framebuffer& operator=(const Framebuffer& other) = delete;

        Framebuffer& operator=(Framebuffer&& other) = delete;

        ~Framebuffer();

        VkFramebuffer get() const;

        VkExtent2D get_extent() const;

    private:
        const VkDevice device_{ VK_NULL_HANDLE };
        VkFramebuffer framebuffer_{ VK_NULL_HANDLE };

        // extent of the attachment in this framebuffer
        const VkExtent2D extent_;
    };
}
