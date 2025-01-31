#include "vulkan/Vertex.h"

namespace njin::vulkan {
    VkVertexInputBindingDescription Vertex::get_binding_description() {
        VkVertexInputBindingDescription binding_description = {
            .binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };

        return binding_description;
    }

    std::vector<VkVertexInputAttributeDescription>
    Vertex::get_attribute_descriptions() {
        std::vector<VkVertexInputAttributeDescription> descriptions{};

        // Position
        VkVertexInputAttributeDescription position_description{
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, position)
        };
        descriptions.push_back(position_description);

        // Color
        VkVertexInputAttributeDescription color_description{
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, color)
        };
        descriptions.push_back(color_description);

        return descriptions;
    }
}  // namespace njin::vulkan