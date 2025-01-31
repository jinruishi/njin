#pragma once

#include <optional>

#include "vulkan/Instance.h"
#include "vulkan/Surface.h"

namespace njin::vulkan {
    /**
     * Stores the indices of queue families with capabilities that we
     * will use
     */
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        /**
         * @return True if each family in the struct has an associated index
         */
        bool is_complete() const;
    };


    /**
    * Wrapper over vkPhy
    */
    class PhysicalDevice {
    public:
        PhysicalDevice(const Instance& instance, const Surface& surface);

        VkPhysicalDevice get() const;

        QueueFamilyIndices get_indices() const;

    private:
        VkPhysicalDevice device_ = VK_NULL_HANDLE;
        QueueFamilyIndices indices_;
    };
}
