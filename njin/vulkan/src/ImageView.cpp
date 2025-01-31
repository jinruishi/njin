#include "vulkan/ImageView.h"

#include <stdexcept>
#include <vulkan/Image.h>

namespace njin::vulkan {
    ImageView::ImageView(const LogicalDevice& device,
                         const Image& image,
                         const VkExtent2D& extent) : device_{ device.get() },
        extent_{ extent } {
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.image = image.get();
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format = image.get_format();
        info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device.get(), &info, nullptr, &image_view_)) {
            throw std::runtime_error("failed to create image view");
        }
    }


    ImageView::~ImageView() {
        vkDestroyImageView(device_, image_view_, nullptr);
    }

    VkImageView ImageView::get() const {
        return image_view_;
    }

    VkExtent2D ImageView::get_extent() const {
        return extent_;
    }
}
