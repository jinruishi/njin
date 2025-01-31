#include "vulkan/PipelineLayout.h"

#include <stdexcept>

namespace njin::vulkan {
    PipelineLayout::PipelineLayout(const LogicalDevice& device) :
        device_{ device.get() } {
        // Descriptor set binding for UBOs
        VkDescriptorSetLayoutBinding ubo_layout_binding{
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            // there will only be one uniform buffer object
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .pImmutableSamplers = nullptr
        };

        // Set up the descriptor layout for the vertex shader
        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = 1,
            .pBindings = &ubo_layout_binding
        };

        VkDescriptorSetLayout descriptor_set_layout{ VK_NULL_HANDLE };
        vkCreateDescriptorSetLayout(device_,
                                    &descriptor_set_layout_info,
                                    nullptr,
                                    &descriptor_set_layout);

        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.setLayoutCount = 1;
        info.pSetLayouts = &descriptor_set_layout;
        info.pushConstantRangeCount = 0;
        info.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(device.get(),
                                   &info,
                                   nullptr,
                                   &pipeline_layout_)) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    PipelineLayout::PipelineLayout(const LogicalDevice& device,
                                   VkPipelineLayoutCreateInfo create_info) :
        device_{ device.get() } {
        if (vkCreatePipelineLayout(device.get(),
                                   &create_info,
                                   nullptr,
                                   &pipeline_layout_)) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    PipelineLayout::PipelineLayout(PipelineLayout&& other) noexcept :
        device_{ other.device_ },
        pipeline_layout_{ other.pipeline_layout_ } {
        other.device_ = VK_NULL_HANDLE;
        other.pipeline_layout_ = VK_NULL_HANDLE;
    }

    PipelineLayout::~PipelineLayout() {
        if (pipeline_layout_ != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
        }
    }

    VkPipelineLayout PipelineLayout::get() const {
        return pipeline_layout_;
    }
}  // namespace njin::vulkan
