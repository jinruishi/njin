#include "vulkan/CommandBuffer.h"

#include <stdexcept>

#include <vulkan/GraphicsPipeline.h>
#include <vulkan/vulkan_core.h>

#include "vulkan/Buffer.h"

namespace njin::vulkan {
    CommandBuffer::CommandBuffer(const LogicalDevice& device,
                                 const CommandPool& command_pool) {
        VkCommandBufferAllocateInfo info{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = command_pool.get(),
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        if (vkAllocateCommandBuffers(device.get(), &info, &command_buffer_) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffer!");
        };
    }

    VkCommandBuffer CommandBuffer::get() const {
        return command_buffer_;
    }

    VkCommandBuffer* CommandBuffer::get_handle_address() {
        return &command_buffer_;
    }

}  // namespace njin::vulkan
