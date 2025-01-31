#include "core/njVertex.h"

#include <cstddef>

namespace njin::core {
    njVertex::njVertex(const njVertexCreateInfo& info) :
        position_{ info.position.has_value() ? info.position.value() :
                                               math::njVec3f{} },
        normal_{ info.normal.has_value() ? info.normal.value() :
                                           math::njVec3f{} },
        tangent_{ info.tangent.has_value() ? info.tangent.value() :
                                             math::njVec4f{} },
        tex_coord_{ info.tex_coord.has_value() ? info.tex_coord.value() :
                                                 math::njVec2f{} } {}

    VkVertexInputBindingDescription njVertex::get_binding_description() {
        VkVertexInputBindingDescription binding_description{
            .binding = 0,
            .stride = sizeof(njVertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };

        return binding_description;
    }

    std::vector<VkVertexInputAttributeDescription>
    njVertex::get_attribute_descriptions() {
        VkVertexInputAttributeDescription position{
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(njVertex, position_)
        };

        VkVertexInputAttributeDescription normal{
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(njVertex, normal_)
        };

        VkVertexInputAttributeDescription tangent{
            .location = 2,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(njVertex, tangent_)
        };

        VkVertexInputAttributeDescription tex_coord{
            .location = 3,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(njVertex, tex_coord_)
        };

        return { position, normal, tangent, tex_coord };
    }
}  // namespace njin::core