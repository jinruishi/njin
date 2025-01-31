#include "vulkan/Renderer.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

#include <vulkan/pipeline_util.h>

#include "vulkan/ShaderModule.h"

namespace njin::core {
    class njVertex;
}  // namespace njin::core

namespace njin::vulkan {
    struct Vertex;

    VkVertexInputBindingDescription Vertex2D::get_binding_description() {
        VkVertexInputBindingDescription binding_description{
            .binding = 0,
            .stride = sizeof(Vertex2D),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };
        return binding_description;
    }

    std::vector<VkVertexInputAttributeDescription>
    Vertex2D::get_attribute_descriptions() {
        std::vector<VkVertexInputAttributeDescription> attribute_descriptions{};
        // position
        VkVertexInputAttributeDescription position_description{
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(Vertex2D, position)
        };
        attribute_descriptions.push_back(position_description);

        // color
        VkVertexInputAttributeDescription color_description{
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex2D, color)
        };

        attribute_descriptions.push_back(color_description);

        return attribute_descriptions;
    }

    Renderer::Renderer(const PhysicalDevice& physical_device,
                       const LogicalDevice& logical_device,
                       Swapchain& swapchain,
                       const RenderPass& render_pass,
                       const GraphicsPipeline& graphics_pipeline) :
        physical_device_{ physical_device },
        logical_device_{ logical_device },
        swapchain_{ swapchain },
        render_pass_{ render_pass },
        graphics_pipeline_{ graphics_pipeline.get() },
        command_pool_{ logical_device_, 0 },
        command_buffer_{ logical_device_, command_pool_ },
        vertex_buffer_{ [&]() -> Buffer {
            Buffer::BufferSettings vertex_buffer_settings{
                .size = 128 * 1024 * 1024,
                .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
            };
            return { logical_device, physical_device, vertex_buffer_settings };
        }() },
        index_buffer_{ [&]() -> Buffer {
            Buffer::BufferSettings index_buffer_settings{
                .size = 128 * 1024 * 1024,
                .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT
            };
            return { logical_device, physical_device, index_buffer_settings };
        }() },
        image_available_semaphore_{ logical_device_ },
        render_finished_semaphore_{ logical_device_ },
        in_flight_fence_{ logical_device_ } {
        init_framebuffers();
    }

    Renderer::~Renderer() {
        vkDeviceWaitIdle(logical_device_.get());
    }

    void Renderer::draw_frame() {
        // wait for the last command buffer to tell us it is done executing
        vkWaitForFences(logical_device_.get(),
                        1,
                        in_flight_fence_.get_handle_address(),
                        VK_TRUE,
                        UINT64_MAX);
        vkResetFences(logical_device_.get(),
                      1,
                      in_flight_fence_.get_handle_address());

        // get the index of the next image that will be available
        // NOTE: this does not mean the image can be written to
        // see https://docs.vulkan.org/spec/latest/chapters/VK_KHR_surface/wsi.html#vkAcquireNextImageKHR
        // "The presentation engine may not have finished reading from the image...
        // ...the application must use semaphore and/or fence..."
        uint32_t image_index{ 0 };
        vkAcquireNextImageKHR(logical_device_.get(),
                              swapchain_.get(),
                              UINT64_MAX,
                              image_available_semaphore_.get(),
                              VK_NULL_HANDLE,
                              &image_index);

        // pointer to framebuffer to use
        Framebuffer* framebuffer{ framebuffers_.at(image_index).get() };

        VkCommandBuffer command_buffer{ command_buffer_.get() };
        VkCommandBufferBeginInfo command_buffer_begin_info{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
        };

        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

        VkClearValue clear_color{ { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        VkOffset2D offset{ 0, 0 };
        VkExtent2D extent{ framebuffer->get_extent() };
        VkRenderPassBeginInfo render_pass_info{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = render_pass_.get(),
            .framebuffer = framebuffer->get(),
            .renderArea = { offset, extent },
            .clearValueCount = 1,
            .pClearValues = &clear_color
        };
        vkCmdBeginRenderPass(command_buffer,
                             &render_pass_info,
                             VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(command_buffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          graphics_pipeline_);

        vkCmdBindIndexBuffer(command_buffer,
                             index_buffer_.get(),
                             0,
                             VK_INDEX_TYPE_UINT32);

        VkBuffer vertex_buffers{ vertex_buffer_.get() };
        VkDeviceSize offsets{ 0 };
        vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffers, &offsets);

        // configure the parts of the pipeline we declared dynamic
        VkViewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(framebuffer->get_extent().width),
            .height = static_cast<float>(framebuffer->get_extent().height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);

        VkRect2D scissor{ .offset = { 0, 0 },
                          .extent = framebuffer->get_extent() };
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);

        // vkCmdDrawIndexed(command_buffer, 3, 1, 0, 0, 0);
        vkCmdDraw(command_buffer, vertex_buffer_.get_element_count(), 1, 0, 0);

        vkCmdEndRenderPass(command_buffer);

        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
            throw std::runtime_error(
            "failed to record command buffer commands");
        }

        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        VkSubmitInfo submit_info{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = image_available_semaphore_.get_handle_address(),
            .pWaitDstStageMask = wait_stages,
            .commandBufferCount = 1,
            .pCommandBuffers = &command_buffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores =
            render_finished_semaphore_.get_handle_address(),
        };

        if (vkQueueSubmit(logical_device_.get_graphics_queue(),
                          1,
                          &submit_info,
                          in_flight_fence_.get()) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit command buffer");
        }

        // present when semaphores signalled (rendering done)
        VkPresentInfoKHR present_info{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = render_finished_semaphore_.get_handle_address(),
            .swapchainCount = 1,
            .pSwapchains = swapchain_.get_handle_address(),
            .pImageIndices = &image_index,
            .pResults = nullptr
        };

        vkQueuePresentKHR(logical_device_.get_present_queue(), &present_info);
    }

    void Renderer::update_vertex_buffer(const std::vector<core::njVertex>&
                                        new_vertices) {
        vertex_buffer_.load(new_vertices);
    }

    void
    Renderer::update_index_buffer(const std::vector<uint16_t>& new_indices) {
        index_buffer_.load(new_indices);
    }

    void Renderer::init_framebuffers() {
        // one framebuffer for each image view in the swapchain
        std::vector<ImageView*> image_views{ swapchain_.get_image_views() };
        for (auto image_view : image_views) {
            framebuffers_
            .emplace_back(std::make_unique<Framebuffer>(logical_device_,
                                                        render_pass_,
                                                        *image_view));
        }
    }
}  // namespace njin::vulkan
