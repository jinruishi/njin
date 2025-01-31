#pragma once
#include <vector>

#include "core/njNode.h"
#include "core/njPrimitive.h"
#include "core/njVertex.h"

namespace njin::core {

    class njMesh {
        public:
        njMesh() = default;
        njMesh(const std::vector<njPrimitive>& primitives);

        /**
         * Get the list of all vertices in this mesh
         * @return List of all vertices
         */
        std::vector<njVertex> get_vertices() const;

        /**
         * Get the list of all primitives in this mesh
         * @return List of primitives
         */
        std::vector<njPrimitive> get_primitives() const;

        /**
         * Associate this mesh with a node, rendering it at that node's location
         * @param node Node to associate with
         */
        void set_node(const njNode* node);

        private:
        std::vector<njPrimitive> primitives_{};
        const njNode* node_{ nullptr };
    };

}  // namespace njin::core
