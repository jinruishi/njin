#include "vulkan/Image.h"

namespace njin::vulkan {
    Image::Image(VkImage image, VkFormat format, const VkExtent2D& extent):
        image_{ image },
        format_{ format },
        extent_{ extent } {};

    VkImage Image::get() const {
        return image_;
    }

    VkFormat Image::get_format() const {
        return format_;
    };
}
