#pragma once
#include <string>
#include <vector>

#include "Accessor.h"
#include "util/Buffer.h"
#include "util/BufferView.h"

namespace njin::gltf {
    /**
    * Representation of a GLTF asset, parsed from a given file
    */
    class GLTFAsset {
        public:
        /**
        * Constructor
        * @param path Path to GLB file
        */
        GLTFAsset(const std::string& path);

        /**
         * Retrieve the array of position attributes for vertices of this mesh
         * @return Array of position attributes
         * @note Returns an empty array if there were no position attributes defined
         */
        std::vector<math::njVec3f> get_position_attributes() const;

        /**
         * Retrieve the array of normal attributes for vertices of this mesh
         * @return Array of normal attributes
         * @note Returns an empty array if there were no normal attributes defined
         */
        std::vector<math::njVec3f> get_normal_attributes() const;

        /**
         * Retrieve the array of tangent attributes for vertices of this mesh
         * @return Array of tangent attributes
         * @note Returns an empty array if there were no tangent attributes defined
         */
        std::vector<math::njVec4f> get_tangent_attributes() const;

        /**
         * Retrieve the array of indices for vertices of this mesh
         * @return Array of indices
         */
        std::vector<uint16_t> get_indices() const;

        private:
        uint32_t length_{ 0 };
        gltf::Buffer buffer_{};
        std::vector<gltf::BufferView> buffer_views_{};
        std::vector<gltf::Accessor> accessors_{};

        std::vector<math::njVec3f> position_attributes_{};
        std::vector<math::njVec3f> normal_attributes_{};
        std::vector<math::njVec4f> tangent_attributes_{};
        std::vector<uint16_t> indices_{};
    };
}  // namespace njin::gltf
