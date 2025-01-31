#pragma once

#include "Buffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/GraphicsPipeline.h"
#include "vulkan/RenderPass.h"

namespace njin::vulkan {
    class CommandBuffer {
        public:
        /**
         * Constructor
         * @param device Logical device to allocatee this command buffer on
         * @param command_pool Command pool to allocate a command buffer from
         * @param count Number of command buffers to allocate from this pool
         */
        CommandBuffer(const LogicalDevice& device,
                      const CommandPool& command_pool);

        CommandBuffer(const CommandBuffer& other) = delete;

        CommandBuffer(CommandBuffer&& other) = delete;

        CommandBuffer& operator=(const CommandBuffer& other) = delete;

        CommandBuffer& operator=(CommandBuffer&& other) = delete;

        VkCommandBuffer get() const;

        VkCommandBuffer* get_handle_address();

        private:
        VkCommandBuffer command_buffer_{ VK_NULL_HANDLE };
    };
}  // namespace njin::vulkan
