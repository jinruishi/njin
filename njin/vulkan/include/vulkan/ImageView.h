#pragma once
#include <vulkan/vulkan_core.h>

#include "Image.h"
#include "LogicalDevice.h"

namespace njin::vulkan {
    class ImageView {
    public:
        ImageView(const LogicalDevice& device,
                  const Image& image,
                  const VkExtent2D& extent);

        ImageView(const ImageView&) = delete;

        ImageView& operator=(const ImageView&) = delete;

        ImageView(ImageView&& other) = delete;

        ImageView& operator=(ImageView&&) = delete;

        ~ImageView();

        VkImageView get() const;

        VkExtent2D get_extent() const;

    private:
        VkImageView image_view_{ VK_NULL_HANDLE };
        VkDevice device_{ VK_NULL_HANDLE };
        const VkExtent2D extent_;
    };
}
