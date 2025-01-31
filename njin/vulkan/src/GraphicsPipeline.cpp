#include "vulkan/GraphicsPipeline.h"

#include <stdexcept>
#include <string>

#include <vulkan/Renderer.h>
#include <vulkan/vulkan_core.h>

#include "vulkan/Buffer.h"
#include "vulkan/ShaderModule.h"
#include "vulkan/Vertex.h"
#include "vulkan/pipeline_util.h"

namespace {
    using namespace njin::vulkan;
}  // namespace

namespace njin::vulkan {
    GraphicsPipeline::GraphicsPipeline(const LogicalDevice& device,
                                       const PipelineLayout& pipeline_layout,
                                       const RenderPass& render_pass) :
        device_{ device.get() } {
        // Create the vertex and fragment shaders
        ShaderModule vertex{ device,
                             SHADER_DIR "/shader.vert",
                             VK_SHADER_STAGE_VERTEX_BIT };

        VkPipelineShaderStageCreateInfo vertex_stage_info{
            make_shader_stage_create_info(vertex)
        };

        ShaderModule fragment{ device,
                               SHADER_DIR "/shader.frag",
                               VK_SHADER_STAGE_FRAGMENT_BIT };

        VkPipelineShaderStageCreateInfo fragment_stage_info{
            make_shader_stage_create_info(fragment)
        };

        std::vector<VkPipelineShaderStageCreateInfo> stage_infos{
            vertex_stage_info,
            fragment_stage_info
        };

        // Vertex input state
        std::vector<VkVertexInputBindingDescription> bindings{
            Vertex::get_binding_description()
        };
        auto attributes{ Vertex::get_attribute_descriptions() };
        VkPipelineVertexInputStateCreateInfo vertex_state_info{
            make_vertex_input_state_create_info(bindings, attributes)
        };

        VkPipelineInputAssemblyStateCreateInfo input_assembly_state_info{
            make_input_assembly_state_create_info(
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        };

        VkPipelineViewportStateCreateInfo viewport_state_info{
            make_viewport_state_create_info()
        };

        VkPipelineRasterizationStateCreateInfo rasterization_state_info{
            make_rasterization_state_create_info()
        };

        VkPipelineMultisampleStateCreateInfo multisample_state_info{
            make_multisample_state_create_info()
        };

        VkPipelineDepthStencilStateCreateInfo depth_stencil_state_info{
            make_depth_stencil_state_create_info()
        };

        std::vector<VkPipelineColorBlendAttachmentState> attachments{};
        VkPipelineColorBlendAttachmentState attachment{
            make_color_blend_attachment_state()
        };
        attachments.push_back(attachment);
        VkPipelineColorBlendStateCreateInfo color_blend_state_info{
            make_color_blend_state_create_info(attachments)
        };

        std::vector<VkDynamicState> dynamic_states{ VK_DYNAMIC_STATE_VIEWPORT,
                                                    VK_DYNAMIC_STATE_SCISSOR };

        VkPipelineDynamicStateCreateInfo dynamic_state_info{
            make_dynamic_state_create_info(dynamic_states)
        };

        VkGraphicsPipelineCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        info.pNext = NULL;
        info.flags = 0;
        info.stageCount = stage_infos.size();
        info.pStages = stage_infos.data();
        info.pVertexInputState = &vertex_state_info;
        info.pInputAssemblyState = &input_assembly_state_info;
        info.pTessellationState = nullptr;
        info.pViewportState = &viewport_state_info;
        info.pRasterizationState = &rasterization_state_info;
        info.pMultisampleState = &multisample_state_info;
        info.pColorBlendState = &color_blend_state_info;
        info.pDynamicState = &dynamic_state_info;
        info.layout = pipeline_layout.get();
        info.renderPass = render_pass.get();
        info.subpass = 0;  // one for now, probably will change
        info.basePipelineHandle = VK_NULL_HANDLE;
        info.basePipelineIndex = -1;

        if (vkCreateGraphicsPipelines(device_,
                                      VK_NULL_HANDLE,
                                      1,
                                      &info,
                                      nullptr,
                                      &pipeline_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }

    GraphicsPipeline::GraphicsPipeline(const LogicalDevice& device,
                                       const VkGraphicsPipelineCreateInfo&
                                       info) :
        device_{ device.get() } {
        if (vkCreateGraphicsPipelines(device_,
                                      VK_NULL_HANDLE,
                                      1,
                                      &info,
                                      nullptr,
                                      &pipeline_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }

    GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) noexcept :
        device_{ other.device_ },
        pipeline_{ other.pipeline_ } {
        other.device_ = VK_NULL_HANDLE;
        other.pipeline_ = VK_NULL_HANDLE;
    }

    GraphicsPipeline::GraphicsPipeline(const LogicalDevice& device,
                                       VkPipeline pipeline) :
        device_{ device.get() },
        pipeline_{ pipeline } {}

    GraphicsPipeline::~GraphicsPipeline() {
        if (pipeline_ != VK_NULL_HANDLE) {
            vkDestroyPipeline(device_, pipeline_, nullptr);
        }
    }

    VkPipeline GraphicsPipeline::get() const {
        return pipeline_;
    }
}  // namespace njin::vulkan
