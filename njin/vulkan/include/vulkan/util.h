#pragma once

#include <set>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

#include "PhysicalDevice.h"
#include "vulkan/LogicalDevice.h"


/**
* Utility functions to interface with the Vulkan API in a cleaner and safer way
* by abstracting away the setup for common use-cases
*/
namespace njin::vulkan {
    /**
     * Get the names of all extensions supported by a given physical device
     * @param physical_device Physical device to query extension support for
     * @return Set of names of extensions supported by the physical device
     */
    std::set<std::string> get_physical_device_extensions(
        const PhysicalDevice& physical_device);


    /**
     * Check if a user-provided list of extension names is supported by the
     * indicated physical device
     * @param physical_device  Physical device to check for extension support
     * @param requested_extensions Names of extensions we require support for
     * @return True if all the extensions given by the user is supported
     */
    bool check_physical_device_extension_support(
        const PhysicalDevice& physical_device,
        const std::vector<const char*>& requested_extensions);


    // Surface queries
    VkSurfaceCapabilitiesKHR get_surface_capabilities(
        const PhysicalDevice& physical_device,
        const Surface& surface);

    std::vector<VkSurfaceFormatKHR> get_surface_formats(
        const PhysicalDevice& physical_device,
        const Surface& surface);

    VkSurfaceFormatKHR choose_surface_format(
        const std::vector<VkSurfaceFormatKHR>& available_formats);


    std::vector<VkPresentModeKHR> get_surface_presentation_modes(
        const PhysicalDevice& physical_device,
        const Surface& surface);

    /**
     * Find the index of a suitable memory type for our requirements on the
     * physical device
     * @param physical_device Physical device to query memory from
     * @param type_filter Memory type indices that can be used
     * (from a vkGetBufferMemoryRequirements call)
     * @param properties User requirements for the memory (visible to host, coherent, etc)
     * @return Suitable memory type index.
     * @throw Exception if no suitable memory type index found
     */
    uint32_t find_memory_type(const PhysicalDevice& physical_device,
                              uint32_t type_filter,
                              VkMemoryPropertyFlags properties);


    /**
    * Make a semaphore
    * @param device Device to make the semaphore for
    * @param semaphore Handle to semaphore
    */
    void make_semaphore(const LogicalDevice& device, VkSemaphore* semaphore);

    /**
    * Make a fence
    * @param device Device to make the fence for
    * @param fence Handle to fence
    */
    void make_fence(const LogicalDevice& device, VkFence* fence);
}
