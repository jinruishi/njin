#pragma once

#include "Buffer.h"
#include "Fence.h"
#include "Semaphore.h"
#include "core/njVertex.h"
#include "math/njVec2.h"
#include "math/njVec3.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/GraphicsPipeline.h"
#include "vulkan/Instance.h"
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"
#include "vulkan/PipelineLayout.h"
#include "vulkan/RenderPass.h"
#include "vulkan/Surface.h"
#include "vulkan/Swapchain.h"
#include "vulkan/Window.h"
#include "vulkan/util.h"

namespace njin::vulkan {
    /**
    * Coordinator for all the Vulkan components
    * Responsible for using them together to draw frames
    */

    struct Vertex2D {
        math::njVec2<float> position;
        math::njVec3<float> color;

        // defaults to binding slot 0
        static VkVertexInputBindingDescription get_binding_description();

        static std::vector<VkVertexInputAttributeDescription>
        get_attribute_descriptions();
    };

    class Renderer {
        public:
        Renderer(const PhysicalDevice& physical_device,
                 const LogicalDevice& logical_device,
                 Swapchain& swapchain,
                 const RenderPass& render_pass,
                 const GraphicsPipeline& graphics_pipeline);

        Renderer(const Renderer& other) = delete;

        Renderer(Renderer&& other) = delete;

        Renderer& operator=(const Renderer& other) = delete;

        Renderer& operator=(Renderer&& other) = delete;

        ~Renderer();

        void draw_frame();

        /**
         * Replace the contents of the vertex buffer with a new set of vertices
         * @param new_vertices Array of new vertices to use
         */
        void
        update_vertex_buffer(const std::vector<core::njVertex>& new_vertices);

        /**
         * Replace the contents of the index buffer with a new set of indices
         * @param new_indices Array of new indices to use
         */
        void update_index_buffer(const std::vector<uint16_t>& new_indices);

        private:
        // TODO: this is not clean, holding const references to objects - yuck
        // also, storing some members as abstracted types and some as direct vk types!?
        const PhysicalDevice& physical_device_;
        const LogicalDevice& logical_device_;
        Swapchain& swapchain_;
        const RenderPass& render_pass_;
        VkPipeline graphics_pipeline_{ VK_NULL_HANDLE };
        std::vector<std::unique_ptr<Framebuffer>> framebuffers_{};
        CommandPool command_pool_;
        CommandBuffer command_buffer_;
        Buffer vertex_buffer_;
        Buffer index_buffer_;

        // sync objects
        Semaphore image_available_semaphore_;
        Semaphore render_finished_semaphore_;
        Fence in_flight_fence_;

        void init_framebuffers();
    };
}  // namespace njin::vulkan
