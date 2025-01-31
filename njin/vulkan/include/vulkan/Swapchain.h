#pragma once

#include <memory>

#include "Image.h"
#include "ImageView.h"
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"

namespace njin::vulkan {
    class Swapchain {
        public:
        Swapchain(const LogicalDevice& device,
                  const PhysicalDevice& physical_device,
                  const Surface& surface);

        Swapchain(const Swapchain&) = delete;

        Swapchain& operator=(const Swapchain&) = delete;

        Swapchain(Swapchain&&) = delete;

        Swapchain& operator=(Swapchain&&) = delete;

        ~Swapchain();

        VkSwapchainKHR get() const;

        VkSwapchainKHR* get_handle_address();

        /**
         * Get the image format of the images managed by this swapchain
         * @return Image format
         */
        VkFormat get_image_format() const;

        /**
         * Get handles to the image views of the images in the swap chain
         * @return List of image view handles
         */
        std::vector<ImageView*> get_image_views() const;

        private:
        VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };
        const VkDevice device_{ VK_NULL_HANDLE };
        std::vector<std::unique_ptr<Image>> images_;
        std::vector<std::unique_ptr<ImageView>> image_views_;
        VkFormat image_format_{ VK_FORMAT_UNDEFINED };
    };
}  // namespace njin::vulkan
