#pragma once
#include "core/njNode.h"

namespace njin::core {
    class njCamera {
        public:
        struct njCameraSettings {
            float far{ 0 };
            float near{ 0 };
            float y_fov{ 0 };
        };

        /**
         * Constructor
         * @param name Name to give this camera object
         * @param settings Camera settings
         */
        njCamera(const std::string& name, const njCameraSettings& settings);

        /**
         * Orient the camera to look at a specified point in local space
         * @param target Local space coordinate to look at
         * @param up Up vector in local space
         */
        void look_at(const math::njVec3f& target, const math::njVec3f& up);

        /**
         * Set the node this camera references
         * @param node Node this camera references
         */
        void set_node(const njNode* node);

        private:
        const std::string name_{};
        float far_{};
        float near_{};
        float y_fov_{};
        const njNode* node_{ nullptr };
    };
}  // namespace njin::core