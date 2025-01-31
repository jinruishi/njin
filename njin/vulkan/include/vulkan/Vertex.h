#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

#include "math/njVec2.h"
#include "math/njVec3.h"

namespace njin::vulkan {
    struct Vertex {
        math::njVec3f position;
        math::njVec3f color;

        static VkVertexInputBindingDescription get_binding_description();

        static std::vector<VkVertexInputAttributeDescription>
        get_attribute_descriptions();
    };
}  // namespace njin::vulkan
