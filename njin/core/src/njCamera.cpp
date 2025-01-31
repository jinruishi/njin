#include "core/njCamera.h"

namespace njin::core {

    njCamera::njCamera(const std::string& name,
                       const njCameraSettings& settings) :
        name_{ name },
        far_{ settings.far },
        near_{ settings.near },
        y_fov_{ settings.y_fov } {}

    void njCamera::look_at(const math::njVec3f& target,
                           const math::njVec3f& up) {}

    void njCamera::set_node(const njNode* node) {
        node_ = node;
    }
}  // namespace njin::core