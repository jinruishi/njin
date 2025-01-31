#include <chrono>
#include <iostream>

#include <vulkan/CommandBuffer.h>
#include <vulkan/GraphicsPipelineBuilder.h>
#include <vulkan/RenderPassBuilder.h>
#include <vulkan/Renderer.h>
#include <vulkan/ShaderModule.h>
#include <vulkan/pipeline_util.h>

#include "core/njVertex.h"
#include "math/njVec3.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/PhysicalDevice.h"
#include "vulkan/Vertex.h"
#include "vulkan/Window.h"

using namespace njin::vulkan;

int main() {
    namespace core = njin::core;

    bool should_run{ true };

    SDL_Init(SDL_INIT_EVENTS);

    Window window{
        "njin",
        1280,
        720,
        SDL_WINDOW_VULKAN,
    };
    auto extensions{ window.get_extensions() };
    Instance instance{ "njin", extensions };
    Surface surface{ instance, window };
    PhysicalDevice physical_device{ instance, surface };
    LogicalDevice logical_device{ physical_device };
    Swapchain swapchain{ logical_device, physical_device, surface };
    PipelineLayout pipeline_layout{ logical_device };
    RenderPassBuilder render_pass_builder(logical_device);

    render_pass_builder.add_attachment_description(
    "color",
    { .flags = 0,
      .format = swapchain.get_image_format(),
      .load_op = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .store_op = VK_ATTACHMENT_STORE_OP_STORE,
      .stencil_load_op = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencil_store_op = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
      .final_layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR });

    AttachmentReference color_attachment_reference{
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT
    };

    std::vector<std::pair<AttachmentId, AttachmentReference>> color_attachments{
        { "color", color_attachment_reference }
    };

    render_pass_builder.add_subpass_description("zero",
                                                { .flags = 0,
                                                  .input_attachments = {},
                                                  .color_attachments =
                                                  color_attachments });

    RenderPass render_pass{ render_pass_builder.build() };

    /** build the graphics pipeline */
    GraphicsPipelineBuilder pipeline_builder{ logical_device,
                                              render_pass,
                                              pipeline_layout };

    /** shader modules */
    ShaderModule vertex_shader_module{ logical_device,
                                       SHADER_DIR "/shader.vert",
                                       VK_SHADER_STAGE_VERTEX_BIT };
    ShaderModule fragment_shader_module{ logical_device,
                                         SHADER_DIR "/shader.frag",
                                         VK_SHADER_STAGE_FRAGMENT_BIT };
    pipeline_builder.add_shader_stage(vertex_shader_module);
    pipeline_builder.add_shader_stage(fragment_shader_module);

    /** vertex input */
    pipeline_builder.add_vertex_input_binding_description(
    core::njVertex::get_binding_description());

    std::vector<VkVertexInputAttributeDescription> attribute_descriptions{
        core::njVertex::get_attribute_descriptions()
    };

    for (const auto& description : attribute_descriptions) {
        pipeline_builder.add_vertex_attribute_description(description);
    }

    pipeline_builder
    .set_primitive_topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    GraphicsPipelineBuilder::RasterizationSettings rasterization_settings{
        .polygon_mode = VK_POLYGON_MODE_FILL,
        .cull_mode = VK_CULL_MODE_BACK_BIT,
        .front_face = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    };

    pipeline_builder.set_rasterization_state(rasterization_settings);

    std::vector<VkDynamicState> dynamic_states{ VK_DYNAMIC_STATE_VIEWPORT,
                                                VK_DYNAMIC_STATE_SCISSOR };
    pipeline_builder.set_dynamic_state(dynamic_states);

    GraphicsPipeline pipeline{ pipeline_builder.build() };

    Renderer renderer{ physical_device,
                       logical_device,
                       swapchain,
                       render_pass,
                       pipeline };

    SDL_Event event;

    while (should_run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                should_run = false;
            }
        }

        core::njVertexCreateInfo info_1{
            .position = njin::math::njVec3{ -0.5f, 0.5f, 0.f }
        };
        core::njVertexCreateInfo info_2{
            .position = njin::math::njVec3{ 0.5f, -0.5f, 0.f }
        };
        core::njVertexCreateInfo info_3{
            .position = njin::math::njVec3{ -0.5f, -0.5f, 0.f }
        };

        core::njVertex one{ info_1 };
        core::njVertex two{ info_2 };
        core::njVertex three{ info_3 };

        renderer.update_vertex_buffer({ one, two, three });
        renderer.draw_frame();
    }
}
