#pragma once
#include <vulkan/vulkan_core.h>

namespace njin::vulkan {
    class Image {
    public:
        /**
         * Constructor
         * @param image Image handle from the swapchain
         * @param format Format of the image handle as defined when creating
         * the swapchain that holds this image
         */
        Image(VkImage image, VkFormat format, const VkExtent2D& extent);

        VkImage get() const;

        VkFormat get_format() const;

    private:
        VkImage image_;
        VkFormat format_;
        const VkExtent2D extent_;
    };
}
