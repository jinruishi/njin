#include "core/njMesh.h"

namespace njin::core {

    njMesh::njMesh(const std::vector<njPrimitive>& primitives) :
        primitives_{ primitives } {}

    std::vector<njVertex> njMesh::get_vertices() const {
        std::vector<njVertex> vertices{};
        for (const auto& primitive : primitives_) {
            std::vector<njVertex> primitive_vertices{
                primitive.get_vertices()
            };
            vertices.insert(vertices.end(),
                            primitive_vertices.begin(),
                            primitive_vertices.end());
        }

        return vertices;
    }

    std::vector<njPrimitive> njMesh::get_primitives() const {
        return primitives_;
    }

    void njMesh::set_node(const njNode* node) {
        node_ = node;
    }
}  // namespace njin::core